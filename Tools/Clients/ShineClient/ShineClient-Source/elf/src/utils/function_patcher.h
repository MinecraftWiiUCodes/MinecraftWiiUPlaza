/****************************************************************************
 * Copyright (C) 2016 Maschell
 * With code from chadderz and dimok
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

#ifndef _FUNCTION_HOOKS_H_
#define _FUNCTION_HOOKS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <gctypes.h>
#include "../common/common.h" // OS_FIRMWARE

// Library handles
#include "../dynamic_libs/aoc_functions.h"
#include "../dynamic_libs/ax_functions.h"
#include "../dynamic_libs/fs_functions.h"
#include "../dynamic_libs/gx2_functions.h"
#include "../dynamic_libs/os_functions.h"
#include "../dynamic_libs/padscore_functions.h"
#include "../dynamic_libs/socket_functions.h"
#include "../dynamic_libs/sys_functions.h"
#include "../dynamic_libs/vpad_functions.h"
/*#include "../dynamic_libs/acp_functions.h"
#include "../dynamic_libs/syshid_functions.h"*/

/* Macros for libs */
#define LIB_CORE_INIT           0
#define LIB_NSYSNET             1
#define LIB_GX2                 2
#define LIB_AOC                 3
#define LIB_AX                  4
#define LIB_FS                  5
#define LIB_OS                  6
#define LIB_PADSCORE            7
#define LIB_SOCKET              8
#define LIB_SYS                 9
#define LIB_VPAD                10
#define LIB_NN_ACP              11
#define LIB_SYSHID              12
#define LIB_VPADBASE            13

// functions types
#define STATIC_FUNCTION         0
#define DYNAMIC_FUNCTION        1

/*enum Library {
	LIB_CORE_INIT,
	LIB_NSYSNET,
	LIB_GX2,
	LIB_AOC,
	LIB_AX,
	LIB_FS,
	LIB_OS,
	LIB_PADSCORE,
	LIB_SOCKET,
	LIB_SYS,
	LIB_VPAD,
	LIB_NN_ACP,
	LIB_SYSHID,
	LIB_VPADBASE
};

enum FunctionTypes {
	STATIC_FUNCTION,
	DYNAMIC_FUNCTION
};*/

// Original code by Chadderz
#define declareFunctionHook(returnType, functionName, ...) \
        returnType (* real_ ## functionName)(__VA_ARGS__) __attribute__((section(".data"))); \
        returnType my_ ## functionName(__VA_ARGS__)

#define declareGameFunctionHook(functionName) { (unsigned int) my_ ## functionName, (unsigned int) &real_ ## functionName, 0, # functionName,0,0,1,0}

#define makeFunctionHook(functionName, library, functionType) { (unsigned int) my_ ## functionName, (unsigned int) &real_ ## functionName, library, # functionName,0,0,functionType,0}

#define FUNCTION_PATCHER_METHOD_STORE_SIZE  7

typedef struct {
	const unsigned int replaceAddress;
	const unsigned int replaceCall;
	const unsigned int library;
	const char functionName[50];
	unsigned int realAddress;
	unsigned int restoreInstruction;
	unsigned char functionType;
	unsigned char alreadyPatched;
} FunctionHook;

void printFunctionHooks(FunctionHook *functionHooks, u32 functionHooksSize);

bool setRealAddress(FunctionHook functionHooks[], int functionHooksSize, const char *functionName, u32 address);

void patchIndividualMethodHooks(FunctionHook *functionHook, int hook_information_size,
								volatile unsigned int *dynamic_method_calls);

void restoreIndividualInstructions(FunctionHook *functionHooks, int hook_information_size);

unsigned int getFunctionAddress(unsigned int library, const char *functionName);

bool isDynamicFunction(unsigned int physicalAddress);

#ifdef __cplusplus
}
#endif

#endif