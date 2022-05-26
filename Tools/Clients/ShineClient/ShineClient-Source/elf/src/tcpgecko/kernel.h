#pragma once

#include "../kernel/syscalls.h"
#include "assertions.h"
#include "../dynamic_libs/os_functions.h"
#include "tcp_gecko.h"
#include "../utils/logger.h"

// TODO Variable size, not hard-coded
unsigned char *kernelCopyBufferOld[DATA_BUFFER_SIZE];
unsigned char *kernelCopyBuffer[sizeof(int)];

void kernelCopyInt(unsigned char *destinationBuffer, unsigned char *sourceBuffer, unsigned int length) {
	memcpy(kernelCopyBuffer, sourceBuffer, length);
	unsigned int destinationAddress = (unsigned int) OSEffectiveToPhysical(destinationBuffer);
	SC0x25_KernelCopyData(destinationAddress, (unsigned int) &kernelCopyBuffer, length);
	DCFlushRange(destinationBuffer, (u32) length);
}

void writeKernelMemory(const void *address, uint32_t value) {
	((int *) kernelCopyBuffer)[0] = value;
	kernelCopyInt((unsigned char *) address, (unsigned char *) kernelCopyBuffer, sizeof(int));
}

void kernelCopyData(unsigned char *destinationBuffer, unsigned char *sourceBuffer, unsigned int length) {
	if (length > DATA_BUFFER_SIZE){OSFatal("Kernel copy buffer size exceeded");}

	memcpy(kernelCopyBufferOld, sourceBuffer, length);
	SC0x25_KernelCopyData((unsigned int) OSEffectiveToPhysical(destinationBuffer), (unsigned int) &kernelCopyBufferOld,length);
	DCFlushRange(destinationBuffer, (u32) length);
}

int readKernelMemory(const void *address) {
	// For addresses in that range use Chadderz' function to avoid crashing
	if (address > (const void *) 0xF0000000) {
		log_print("Using Chadderz' kern_read()...\n");
		return kern_read(address);
	}

	log_print("Using dimok's kernelCopy()...\n");
	unsigned char *readBuffer[sizeof(int)];
	kernelCopyInt((unsigned char *) readBuffer, (unsigned char *) address, sizeof(int));

	return ((int *) readBuffer)[0];
}

#define KERNEL_COPY_SOURCE_ADDRESS 0x10100000

s32 kernelCopyService(s32 argc, void *argv) {
	while (true) {
		// Read the destination address from the source address
		int destinationAddress = *(int *) KERNEL_COPY_SOURCE_ADDRESS;

		// Avoid crashing
		if (OSIsAddressValid((const void *) destinationAddress)) {
			// Perform memory copy
			unsigned char *valueBuffer = (unsigned char *) (KERNEL_COPY_SOURCE_ADDRESS + 4);
			kernelCopyInt((unsigned char *) destinationAddress, valueBuffer, 4);

			// "Consume" address and value for synchronization with the code handler for instance
			*(int *) KERNEL_COPY_SOURCE_ADDRESS = 0;
			*(((int *) KERNEL_COPY_SOURCE_ADDRESS) + 1) = 0;
		}
	}

	// This is here to avoid the warning
	return 0;
}

void startKernelCopyService() {
	unsigned int stack = (unsigned int) memalign(0x40, 0x100);

	if (stack != 0) {
		stack += 0x100;
		void *thread = memalign(0x40, 0x1000);
		ASSERT_ALLOCATED(thread, "Kernel copy thread")

		int status = OSCreateThread(thread, kernelCopyService, 1, NULL, (u32) stack + sizeof(stack), sizeof(stack), 31,
									OS_THREAD_ATTR_AFFINITY_CORE1 | OS_THREAD_ATTR_PINNED_AFFINITY |
									OS_THREAD_ATTR_DETACH);
		ASSERT_INTEGER(status, 1, "Creating kernel copy thread")
		// OSSetThreadName(thread, "Kernel Copier");
		OSResumeThread(thread);
	}
}

#define MINIMUM_KERNEL_COMPARE_LENGTH 4
#define KERNEL_MEMORY_COMPARE_STEP_SIZE 1

int kernelMemoryCompare(const char *sourceBuffer,
						const char *destinationBuffer,
						int length) {
	if (length < MINIMUM_KERNEL_COMPARE_LENGTH) {
		ASSERT_MINIMUM_HOLDS(length, MINIMUM_KERNEL_COMPARE_LENGTH, "length");
	}

	bool loopEntered = false;
	while (kern_read(sourceBuffer) == kern_read(destinationBuffer)) {
		loopEntered = true;
		sourceBuffer = (char *) sourceBuffer + KERNEL_MEMORY_COMPARE_STEP_SIZE;
		destinationBuffer = (char *) destinationBuffer + KERNEL_MEMORY_COMPARE_STEP_SIZE;
		length -= KERNEL_MEMORY_COMPARE_STEP_SIZE;

		if (length <= MINIMUM_KERNEL_COMPARE_LENGTH - 1) {
			break;
		}
	}

	if (loopEntered) {
		sourceBuffer -= KERNEL_MEMORY_COMPARE_STEP_SIZE;
		destinationBuffer -= KERNEL_MEMORY_COMPARE_STEP_SIZE;
	}

	return kern_read(sourceBuffer) - kern_read(destinationBuffer);
}

void executeAssembly(unsigned char buffer[], unsigned int size) {
	// Write the assembly to an executable code region
	int destinationAddress = 0x10000000 - size;
	kernelCopyData((unsigned char *) destinationAddress, buffer, size);

	// Execute the assembly from there
	void (*function)() = (void (*)()) destinationAddress;
	function();

	// Clear the memory contents again
	memset((void *) buffer, 0, size);
	kernelCopyData((unsigned char *) destinationAddress, buffer, size);
}