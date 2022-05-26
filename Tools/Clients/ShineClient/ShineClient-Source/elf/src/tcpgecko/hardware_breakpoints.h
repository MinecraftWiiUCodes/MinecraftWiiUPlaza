#include <stdbool.h>
#include "../dynamic_libs/os_functions.h"
#include "../system/exception_handler.h"
#include "../kernel/syscalls.h"
#include "../common/common.h"
#include "../utils/logger.h"
#include "software_breakpoints.h"

#ifndef TCPGECKO_HARDWARE_BREAKPOINTS_H
#define TCPGECKO_HARDWARE_BREAKPOINTS_H

#ifdef __cplusplus
extern "C" {
#endif

/*	Fix a gap in exception_handler.h
    Yes, this is the dsisr.
*/
#define dsisr exception_specific0
#define DSISR_DABR_MATCH 0x400000

extern /* kernelmode */ void KernelSetDABR(unsigned int dabr);

extern void SC0x2D_KernelSetDABR(unsigned int dabr);

static void RegisterDataBreakpointHandler(unsigned char (*breakpointHandler)(OSContext *ctx));

static void DataBreakpoints_Install();

static unsigned char DataBreakpoints_DSIHandler(void *ctx);

static unsigned char (*bHandler)(OSContext *ctx);

static void RegisterDataBreakpointHandler(unsigned char (*breakpointHandler)(OSContext *ctx)) {
	bHandler = breakpointHandler;
}

static inline int getDABRAddress(void *interruptedContext) {
	OSContext *context = (OSContext *) interruptedContext;
	return (int) context->srr0; // Offset 0xA4
}

unsigned char basicDABRBreakpointHandler(OSContext *context) {
	// log_print("Getting DABR address\n");
	int address = getDABRAddress(context);
	// log_printf("Got DABR address: %08x\n", address);

	if (OSIsAddressValid((const void *) address)) {
		// log_printf("Data breakpoint address: %x08\n", address);
	} else {
		// log_printf("Data breakpoint invalid address: %x08\n", address);
	}

	return 0;
}

static void SetDataBreakpoint(unsigned int address, bool read, bool write) {
	// log_print("Before installing...\n");
	DataBreakpoints_Install();
	// log_print("After installing...\n");
	RegisterDataBreakpointHandler(basicDABRBreakpointHandler);
	// log_print("After registering...\n");

	unsigned int dabr = address & ~0b00000111; //GCC \o/
	// log_printf("DABR 1: %08x\n", dabr);
	dabr |= 0b00000100; //enable translation
	// log_printf("DABR 2: %08x\n", dabr);
	if (read) {
		dabr |= 0b00000001; //break on read
		// log_printf("DABR 3: %08x\n", dabr);
	}
	if (write) {
		dabr |= 0b00000010; //break on write
		// log_printf("DABR 4: %08x\n", dabr);
	}

	// log_print("Setting DABR...\n");
	SC0x2D_KernelSetDABR(dabr);
	// log_print("DABR set!\n");
}

static unsigned char DataBreakpoints_DSIHandler(void *ctx) {
	// log_print("DSI handler\n");
	/*OSContext *context = (OSContext *) ctx;
	if (context->dsisr & DSISR_DABR_MATCH) {
		// log_print("Running BP handler\n");
		if (bHandler) {
			return bHandler(context);
		}
	}*/

	// log_print("DSI exception\n");
	return dsi_exception_cb(ctx);
}

static void DataBreakpoints_Install() {
	kern_write((void *) (OS_SPECIFICS->addr_KernSyscallTbl1 + (0x2D * 4)), (unsigned int) &KernelSetDABR);
	kern_write((void *) (OS_SPECIFICS->addr_KernSyscallTbl2 + (0x2D * 4)), (unsigned int) &KernelSetDABR);
	kern_write((void *) (OS_SPECIFICS->addr_KernSyscallTbl3 + (0x2D * 4)), (unsigned int) &KernelSetDABR);
	kern_write((void *) (OS_SPECIFICS->addr_KernSyscallTbl4 + (0x2D * 4)), (unsigned int) &KernelSetDABR);
	kern_write((void *) (OS_SPECIFICS->addr_KernSyscallTbl5 + (0x2D * 4)), (unsigned int) &KernelSetDABR);

	OSSetExceptionCallback((u8) OS_EXCEPTION_DSI, &DataBreakpoints_DSIHandler);
}

// Special purpose registers
#define IABR 0x3F2
#define DABR 0x3F5

// http://www.ds.ewi.tudelft.nl/vakken/in1006/instruction-set/mtspr.html
#define mtspr(spr, value)                        \
    __asm__ __volatile__ ("mtspr %0, %1" : : "K" (spr), "r" (value))    \


// https://www.ibm.com/support/knowledgecenter/en/ssw_aix_71/com.ibm.aix.alangref/idalangref_isync_ics_instrs.htm
static inline void isync() {
	__asm__ __volatile__ ("isync" : : : "memory");
}

// https://www.ibm.com/support/knowledgecenter/en/ssw_aix_61/com.ibm.aix.alangref/idalangref_eieio_instrs.htm
static inline void eieio() {
	__asm__ __volatile__ ("eieio" : : : "memory");
}

// https://www.ibm.com/support/knowledgecenter/ssw_aix_71/com.ibm.aix.alangref/idalangref_rfi_retfinter_instrs.htm
static inline void rfi() {
	__asm__ __volatile__ ("rfi" : : : "memory");
}

// https://www.manualslib.com/manual/606065/Ibm-Powerpc-750gx.html?page=64
static inline void setIABR(unsigned int address) {
	mtspr(IABR, address);
	eieio();
	isync();
}

static inline int getIABRAddress() {
	return mfspr(IABR);
}

static inline int getIABRMatch(void *interruptedContext) {
	OSContext *context = (OSContext *) interruptedContext;
	return (int) context->exception_specific1; // Offset 0x98
}

unsigned char breakPointHandler(void *interruptedContext);

void registerBreakPointHandler() {
	// log_print("Registering breakpoint handler...\n");
	// TODO Not working, never called?
	// OSSetExceptionCallback((u8) OS_EXCEPTION_DSI, &breakPointHandler);
	// OSSetExceptionCallback((u8) OS_EXCEPTION_ISI, &breakPointHandler);
	// OSSetExceptionCallback((u8) OS_EXCEPTION_PROGRAM, &breakPointHandler);
	OSSetExceptionCallbackEx((u8) OS_EXCEPTION_MODE_GLOBAL_ALL_CORES, (u8) OS_EXCEPTION_PROGRAM, &breakPointHandler);
	// __OSSetInterruptHandler((u8) OS_EXCEPTION_PROGRAM, &breakPointHandler);
	// log_print("Breakpoint handler(s) registered!\n");
}

/*void forceDebuggerInitialized() {
	unsigned char patchBytes[] = {0x38, 0x60, 0x00, 0x01};
	patchFunction(OSIsDebuggerInitialized, (char *) patchBytes, sizeof(patchBytes), 0x1C);
}

void forceDebuggerPresent() {
	unsigned char patchBytes[] = {0x38, 0x60, 0x00, 0x01, 0x60, 0x00, 0x00, 0x00};
	patchFunction(OSIsDebuggerPresent, (char *) patchBytes, sizeof(patchBytes), 0x0);
}*/

static inline void setupBreakpointSupport() {
	/*// log_print("Clear and enable...\n");
	__OSClearAndEnableInterrupt();
	// log_print("Restore...\n");
	OSRestoreInterrupts();
	// log_print("Enable...\n");
	OSEnableInterrupts();
	forceDebuggerPresent();
	forceDebuggerInitialized();*/

	registerBreakPointHandler();
}

void setDataBreakpoint(int address, bool read, bool write) {
	setupBreakpointSupport();
	// log_print("Setting DABR...\n");
	OSSetDABR(1, address, read, write);
	// log_print("DABR set\n");
	// int enabled = OSIsInterruptEnabled();
	// log_printf("Interrupts enabled: %i\n", enabled);
}

void setInstructionBreakpoint(unsigned int address) {
	setupBreakpointSupport();

	// int returnedAddress;

	// log_print("Setting IABR #1...\n");
	// OSSetIABR(1, address);
	setIABR(address);
	// log_print("IABR set #1...\n");
	/*
	// TODO Causes crash
	returnedAddress = getIABRAddress();
	// log_printf("IABR spr value: %08x\n", returnedAddress);

	// log_print("Setting IABR #2...\n");
	setIABR(address);
	// log_print("IABR set #2...\n");
	returnedAddress = mfspr(IABR);
	// log_printf("IABR spr value: %08x\n", returnedAddress);*/
}

unsigned char breakPointHandler(void *interruptedContext) {
	// Check for data breakpoints
	int dataAddress = getDABRAddress(interruptedContext);
	if (OSIsAddressValid((const void *) dataAddress)) {
		// log_printf("Data breakpoint address: %x08\n", dataAddress);
	} else {
		// log_printf("Data breakpoint invalid address: %x08\n", dataAddress);

		// Check for instruction breakpoints
		int instructionAddress = getIABRMatch(interruptedContext);
		if (OSIsAddressValid((const void *) instructionAddress)) {
			// log_printf("Instruction breakpoint address: %x08\n", dataAddress);
		} else {
			// log_print("Instruction breakpoint failed!\n");
		}
	}

	setDataBreakpoint(0, false, false);
	setInstructionBreakpoint(0);

	rfi();

	return 0;
}

#ifdef __cplusplus
} //extern "C"
#endif

#endif //TCPGECKO_HARDWARE_BREAKPOINTS_H
