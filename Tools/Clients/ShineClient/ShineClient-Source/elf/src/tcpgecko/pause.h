#pragma once

#include "../utils/logger.h"
#include "assertions.h"
#include "../dynamic_libs/os_functions.h"
#include "../common/fs_defs.h"
#include "kernel.h"

int (*AVMGetDRCScanMode)(int);

unsigned long getConsoleStatePatchAddress() {
	if (AVMGetDRCScanMode) {
		log_print("Already acquired!\n");
	} else {
		// Acquire the RPL and function
		log_print("Acquiring...\n");
		u32 avm_handle;
		OSDynLoad_Acquire("avm.rpl", &avm_handle);
		ASSERT_ALLOCATED(avm_handle, "avm.rpl")
		OSDynLoad_FindExport((u32) avm_handle, 0, "AVMGetDRCScanMode", &AVMGetDRCScanMode);
		ASSERT_ALLOCATED(AVMGetDRCScanMode, "AVMGetDRCScanMode")
		log_print("Acquired!\n");
	}

	return (unsigned long) ((char *) AVMGetDRCScanMode + 0x44);
}

typedef enum {
	PAUSED = 0x38000001,
	RUNNING = 0x38000000
} ConsoleState;

void writeConsoleState(ConsoleState state) {
	// Get the value to write
	int patchValue = state;
	log_printf("Patch value: %x\n", patchValue);

	// Write the value
	unsigned int patchAddress = getConsoleStatePatchAddress();
	log_printf("Patch address: %x\n", patchAddress);
	kernelCopyData((unsigned char *) patchAddress, (unsigned char *) &patchValue, 4);
}

bool isConsolePaused() {
	unsigned int patchAddress = getConsoleStatePatchAddress();
	log_printf("Patch address: %x\n", patchAddress);
	int value = *(unsigned int *) patchAddress;

	return value == PAUSED;
}