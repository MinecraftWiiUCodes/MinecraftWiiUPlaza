#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <fcntl.h>
#include "../dynamic_libs/os_functions.h"
#include "../dynamic_libs/fs_functions.h"
#include "../dynamic_libs/sys_functions.h"
#include "../dynamic_libs/vpad_functions.h"
#include "../dynamic_libs/socket_functions.h"
#include "../kernel/kernel_functions.h"
#include "../system/memory.h"
#include "../common/common.h"
#include "main.h"
#include "code_handler.h"
#include "../utils/logger.h"
#include "../utils/function_patcher.h"
#include "../patcher/function_patcher_gx2.h"
#include "../patcher/function_patcher_coreinit.h"
#include "sd_ip_reader.h"
#include "title.h"
#include "tcp_gecko.h"

bool isCodeHandlerInstalled = false;
bool areSDCheatsEnabled = false;

typedef enum {
	EXIT,
	TCP_GECKO
} LaunchMethod;

void applyFunctionPatches() {
	patchIndividualMethodHooks(method_hooks_gx2, method_hooks_size_gx2, method_calls_gx2);
	patchIndividualMethodHooks(method_hooks_coreinit, method_hooks_size_coreinit, method_calls_coreinit);
}

void installCodeHandler() {
	unsigned int physicalCodeHandlerAddress = (unsigned int) OSEffectiveToPhysical(
			(void *) CODE_HANDLER_INSTALL_ADDRESS);
	SC0x25_KernelCopyData((u32) physicalCodeHandlerAddress, (unsigned int) codeHandler, codeHandlerLength);
	DCFlushRange((const void *) CODE_HANDLER_INSTALL_ADDRESS, (u32) codeHandlerLength);
	isCodeHandlerInstalled = true;
}

unsigned char *screenBuffer;

#define PRINT_TEXT(x, y, ...) { snprintf(messageBuffer, 80, __VA_ARGS__); OSScreenPutFontEx(0, x, y, messageBuffer); OSScreenPutFontEx(1, x, y, messageBuffer); }

void initializeScreen() {
	// Init screen and screen buffers
	OSScreenInit();
	unsigned int screenBuffer0Size = OSScreenGetBufferSizeEx(0);
	unsigned int screenBuffer1Size = OSScreenGetBufferSizeEx(1);

	screenBuffer = (unsigned char *) MEM1_alloc(screenBuffer0Size + screenBuffer1Size, 0x40);

	OSScreenSetBufferEx(0, screenBuffer);
	OSScreenSetBufferEx(1, (screenBuffer + screenBuffer0Size));

	OSScreenEnableEx(0, 1);
	OSScreenEnableEx(1, 1);
}

void install() {
	installCodeHandler();
	initializeUDPLog();
	log_print("Patching functions\n");
	applyFunctionPatches();
}

/* Entry point */
int Menu_Main(void) {
	//!*******************************************************************
	//!                   Initialize function pointers                   *
	//!*******************************************************************
	//! do OS (for acquire) and sockets first so we got logging
	InitOSFunctionPointers();
	InitSocketFunctionPointers();
	InitFSFunctionPointers();
	InitVPadFunctionPointers();
	InitSysFunctionPointers();

	if (strcasecmp("men.rpx", cosAppXmlInfoStruct.rpx_name) == 0) {
		return EXIT_RELAUNCH_ON_LOAD;
	} else if (strlen(cosAppXmlInfoStruct.rpx_name) > 0 &&
			   strcasecmp("ffl_app.rpx", cosAppXmlInfoStruct.rpx_name) != 0) {

		return EXIT_RELAUNCH_ON_LOAD;
	}

	//! *******************************************************************
	//! *                     Setup EABI registers                        *
	//! *******************************************************************
	register int old_sdata_start, old_sdata2_start;
	asm volatile (
	"mr %0, 13\n"
			"mr %1, 2\n"
			"lis 2, __sdata2_start@h\n"
			"ori 2, 2,__sdata2_start@l\n" // Set the Small Data 2 (Read Only) base register.
			"lis 13, __sdata_start@h\n"
			"ori 13, 13, __sdata_start@l\n"// # Set the Small Data (Read\Write) base register.
	: "=r" (old_sdata_start), "=r" (old_sdata2_start)
	);

	//!*******************************************************************
	//!                    Initialize BSS sections                       *
	//!*******************************************************************
	asm volatile (
	"lis 3, __bss_start@h\n"
			"ori 3, 3,__bss_start@l\n"
			"lis 5, __bss_end@h\n"
			"ori 5, 5, __bss_end@l\n"
			"subf 5, 3, 5\n"
			"li 4, 0\n"
			"bl memset\n"
	);

	SetupKernelCallback();
	// PatchMethodHooks();

	memoryInitialize();
	VPADInit();
	initializeScreen();

	char messageBuffer[80];
	int launchMethod;
	int shouldUpdateScreen = 1;
	s32 vpadError = -1;
	VPADData vpad_data;

	while (true) {
		VPADRead(0, &vpad_data, 1, &vpadError);

		if (shouldUpdateScreen) {
			OSScreenClearBufferEx(0, 0);
			OSScreenClearBufferEx(1, 0);

			InitSocketFunctionPointers();

			char ipAddressMessageBuffer[64];
			__os_snprintf(ipAddressMessageBuffer, 64, "TCP Gecko's IP address: %i.%i.%i.%i",
						  (hostIpAddress >> 24) & 0xFF, (hostIpAddress >> 16) & 0xFF, (hostIpAddress >> 8) & 0xFF,
						  hostIpAddress & 0xFF);

			PRINT_TEXT(0, 0, "ShineClient v3.9.C")
			PRINT_TEXT(0, 1, ipAddressMessageBuffer)

			OSScreenFlipBuffersEx(0);
			OSScreenFlipBuffersEx(1);
		}

		install();
		launchMethod = TCP_GECKO;
		areSDCheatsEnabled = true;
		break;
	}

	asm volatile ("mr 13, %0" : : "r" (old_sdata_start));
	asm volatile ("mr 2,  %0" : : "r" (old_sdata2_start));

	MEM1_free(screenBuffer);

	memoryRelease();

	SYSLaunchTitle(0x00050000101DBE00);
	return EXIT_RELAUNCH_ON_LOAD;
}