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

#include <string.h>
#include "../utils/logger.h"
#include "../kernel/kernel_functions.h"
#include "function_patcher.h"

#define CODE_RW_BASE_OFFSET                             0x00000000
#define DEBUG_LOG_DYN                                   1

void printFunctionHooks(FunctionHook *functionHooks, u32 functionHooksSize) {
	for (unsigned int functionHookIndex = 0; functionHookIndex < functionHooksSize; functionHookIndex++) {
		log_printf("Real address (%s): %08x\n", functionHooks[functionHookIndex].functionName,
				   functionHooks[functionHookIndex].realAddress);
	}
}

bool setRealAddress(FunctionHook functionHooks[], int functionHooksSize, const char *functionName, u32 address) {
	for (int functionHookIndex = 0; functionHookIndex < functionHooksSize; functionHookIndex++) {
		if (strncmp(functionName, functionHooks[functionHookIndex].functionName, 50) == 0) {
			functionHooks[functionHookIndex].realAddress = address;
			log_printf("Function %s defined with address %08x\n", functionHooks[functionHookIndex].functionName,
					   functionHooks[functionHookIndex].realAddress);
			// printFunctionHooks(functionHooks, functionHooksSize);

			return true;
		}
	}

	return false;
}

/*
* Patches a function that is loaded at the start of each application. Its not required to restore, at least when they are really dynamic.
* "normal" functions should be patch with the normal patcher. Current Code by Maschell with the help of dimok. Orignal code by Chadderz.
*/
void patchIndividualMethodHooks(FunctionHook *functionHook, int hook_information_size,
								volatile unsigned int *dynamic_method_calls) {
	log_printf("Patching %d given functions\n", hook_information_size);
	/* Patch branches to it.  */
	volatile unsigned int *space = &dynamic_method_calls[0];

	int method_hooks_count = hook_information_size;

	u32 skipInstructionLength = 1;
	u32 myInstructionLength = 6;
	u32 instructionLength = myInstructionLength + skipInstructionLength;
	u32 flush_len = 4 * instructionLength;
	for (int functionIndex = 0; functionIndex < method_hooks_count; functionIndex++) {
		log_printf("Patching %s...\n", functionHook[functionIndex].functionName);
		if (functionHook[functionIndex].functionType == STATIC_FUNCTION &&
			functionHook[functionIndex].alreadyPatched == 1) {
			if (isDynamicFunction((u32) OSEffectiveToPhysical((void *) functionHook[functionIndex].realAddress))) {
				log_printf("The function %s is a dynamic function. Please fix that <3\n",
						   functionHook[functionIndex].functionName);
				functionHook[functionIndex].functionType = DYNAMIC_FUNCTION;
			} else {
				log_printf("Skipping %s, its already patched\n", functionHook[functionIndex].functionName);
				space += instructionLength;
				continue;
			}
		}

		u32 physical = 0;
		unsigned int replaceAddress = functionHook[functionIndex].replaceAddress;
		unsigned int callAddress = functionHook[functionIndex].replaceCall;

		unsigned int realAddress = functionHook[functionIndex].realAddress;

		if (realAddress == 0) {
			log_printf("The real address was NULL, we need to find it.\n");
			realAddress = getFunctionAddress(functionHook[functionIndex].library,
											 functionHook[functionIndex].functionName);
		} else {
			log_printf("The real address was not NULL! We patch it by the given address.\n");
		}

		if (realAddress == 0) {
			log_printf("[Patch] OSDynLoad_FindExport failed for %s\n", functionHook[functionIndex].functionName);
			space += instructionLength;
			continue;
		}

		if (DEBUG_LOG_DYN)log_printf("%s is located at %08X!\n", functionHook[functionIndex].functionName, realAddress);

		physical = (u32) OSEffectiveToPhysical((void *) realAddress);
		if (!physical) {
			log_printf("Error. Something is wrong with the physical address\n");
			space += instructionLength;
			continue;
		}

		if (DEBUG_LOG_DYN)
			log_printf("%s physical is located at %08X!\n", functionHook[functionIndex].functionName, physical);

		*(volatile unsigned int *) (callAddress) = (unsigned int) (space) - CODE_RW_BASE_OFFSET;

		SC0x25_KernelCopyData((u32) space, physical, 4);
		space++;

		//Only works if skip_instr == 1
		functionHook[functionIndex].realAddress = realAddress;
		functionHook[functionIndex].restoreInstruction = *(space - 1);
		if (DEBUG_LOG_DYN)log_printf("method_hooks[i].realAddr = %08X!\n", functionHook[functionIndex].realAddress);
		if (DEBUG_LOG_DYN)
			log_printf("method_hooks[i].restoreInstruction = %08X!\n", functionHook[functionIndex].restoreInstruction);

		//adding jump to real function thx @ dimok for the assembler code
		/*
			90 61 ff e0     stw     r3,-32(r1)
			3c 60 12 34     lis     r3,4660
			60 63 56 78     ori     r3,r3,22136
			7c 69 03 a6     mtctr   r3
			80 61 ff e0     lwz     r3,-32(r1)
			4e 80 04 20     bctr*/
		*space = 0x9061FFE0;
		space++;
		*space = 0x3C600000 | (((realAddress + (skipInstructionLength * 4)) >> 16) & 0x0000FFFF); // lis r3, real_addr@h
		space++;
		*space = 0x60630000 | ((realAddress + (skipInstructionLength * 4)) & 0x0000ffff); // ori r3, r3, real_addr@l
		space++;
		*space = 0x7C6903A6; // mtctr   r3
		space++;
		*space = 0x8061FFE0; // lwz     r3,-32(r1)
		space++;
		*space = 0x4E800420; // bctr
		space++;
		DCFlushRange((void *) (space - instructionLength), flush_len);
		ICInvalidateRange((unsigned char *) (space - instructionLength), flush_len);

		//setting jump back
		unsigned int replace_instr = 0x48000002 | (replaceAddress & 0x03fffffc);
		DCFlushRange(&replace_instr, 4);

		SC0x25_KernelCopyData(physical, (u32) OSEffectiveToPhysical(&replace_instr), 4);
		ICInvalidateRange((void *) (realAddress), 4);

		functionHook[functionIndex].alreadyPatched = 1;
		log_printf("done!\n");
	}

	log_print("Done with patching given functions!\n");
}

void restoreIndividualInstructions(FunctionHook *functionHooks, int hook_information_size) {
	log_printf("Restoring given functions!\n");
	int method_hooks_count = hook_information_size;
	for (int functionIndex = 0; functionIndex < method_hooks_count; functionIndex++) {
		log_printf("Restoring %s... ", functionHooks[functionIndex].functionName);
		if (functionHooks[functionIndex].restoreInstruction == 0 || functionHooks[functionIndex].realAddress == 0) {
			log_printf("I don't have the information for the restore =( skip\n");
			continue;
		}

		unsigned int realAddress = functionHooks[functionIndex].realAddress;

		if (realAddress == 0) {
			log_printf("The real address was NULL, we need to find it.\n");
			realAddress = getFunctionAddress(functionHooks[functionIndex].library,
											 functionHooks[functionIndex].functionName);
		} else {
			log_printf("The real address was not NULL! We patch it by the given address.\n");
		}

		if (!realAddress) {
			log_printf("[Restore] OSDynLoad_FindExport failed for %s\n", functionHooks[functionIndex].functionName);
			continue;
		}

		u32 physical = (u32) OSEffectiveToPhysical((void *) realAddress);
		if (!physical) {
			log_printf("Something is wrong with the physical address\n");
			continue;
		}

		if (isDynamicFunction((unsigned int) physical)) {
			log_printf("Its a dynamic function. We don't need to restore it!\n",
					   functionHooks[functionIndex].functionName);
		} else {
			physical = (u32) OSEffectiveToPhysical(
					(void *) functionHooks[functionIndex].realAddress); //When its an static function, we need to use the old location
			if (DEBUG_LOG_DYN)
				log_printf("Restoring %08X to %08X\n", (u32) functionHooks[functionIndex].restoreInstruction, physical);
			SC0x25_KernelCopyData(physical, (u32) &functionHooks[functionIndex].restoreInstruction, 4);
			if (DEBUG_LOG_DYN)log_printf("ICInvalidateRange %08X\n", (void *) functionHooks[functionIndex].realAddress);
			ICInvalidateRange((void *) functionHooks[functionIndex].realAddress, 4);
			log_printf("done\n");
		}
		functionHooks[functionIndex].alreadyPatched = 0; // In case a
	}

	KernelRestoreInstructions();
	log_print("Done with restoring given functions!\n");
}

bool isDynamicFunction(unsigned int physicalAddress) {
	return (physicalAddress & 0x80000000) == 0x80000000;
}

unsigned int getRPLHandle(int library, const char *functionName) {
	unsigned int rplHandle = 0;
	u32 gx2_handle = 0;

	switch (library) {
		case LIB_CORE_INIT:
			if (DEBUG_LOG_DYN)log_printf("FindExport of %s! From LIB_CORE_INIT\n", functionName);
			if (coreinit_handle == 0) {
				log_print("LIB_CORE_INIT not acquired\n");
				return 0;
			}
			rplHandle = coreinit_handle;
			break;

		case LIB_GX2:
			if (DEBUG_LOG_DYN)log_printf("FindExport of %s! From LIB_GX2\n", functionName);
			OSDynLoad_Acquire("gx2.rpl", &gx2_handle);
			if (gx2_handle == 0) {
				log_print("LIB_GX2 not acquired\n");
				return 0;
			}
			rplHandle = gx2_handle;
			break;

			/*case LIB_NSYSNET:
				if (DEBUG_LOG_DYN)log_printf("FindExport of %s! From LIB_NSYSNET\n", functionName);
				if (nsysnet_handle == 0) {
					log_print("LIB_NSYSNET not acquired\n");
					return 0;
				}
				rplHandle = nsysnet_handle;
				break;

			case LIB_AOC:
				if (DEBUG_LOG_DYN)log_printf("FindExport of %s! From LIB_AOC\n", functionName);
				if (aoc_handle == 0) {
					log_print("LIB_AOC not acquired\n");
					return 0;
				}
				rplHandle = aoc_handle;
				break;

			case LIB_AX:
				if (DEBUG_LOG_DYN)log_printf("FindExport of %s! From LIB_AX\n", functionName);
				if (sound_handle == 0) {
					log_print("LIB_AX not acquired\n");
					return 0;
				}
				rplHandle = sound_handle;
				break;*/

		case LIB_FS:
			if (DEBUG_LOG_DYN)log_printf("FindExport of %s! From LIB_FS\n", functionName);
			if (coreinit_handle == 0) {
				log_print("LIB_FS not acquired\n");
				return 0;
			}
			rplHandle = coreinit_handle;
			break;

		case LIB_OS:
			if (DEBUG_LOG_DYN)log_printf("FindExport of %s! From LIB_OS\n", functionName);
			if (coreinit_handle == 0) {
				log_print("LIB_OS not acquired\n");
				return 0;
			}
			rplHandle = coreinit_handle;
			break;

			/*
		case LIB_PADSCORE:
			if (DEBUG_LOG_DYN)log_printf("FindExport of %s! From LIB_PADSCORE\n", functionName);
			if (padscore_handle == 0) {
				log_print("LIB_PADSCORE not acquired\n");
				return 0;
			}
			rplHandle = padscore_handle;
			break;

		case LIB_SOCKET:
			if (DEBUG_LOG_DYN)log_printf("FindExport of %s! From LIB_SOCKET\n", functionName);
			if (nsysnet_handle == 0) {
				log_print("LIB_SOCKET not acquired\n");
				return 0;
			}
			rplHandle = nsysnet_handle;
			break;

		case LIB_SYS:
			if (DEBUG_LOG_DYN)log_printf("FindExport of %s! From LIB_SYS\n", functionName);
			if (sysapp_handle == 0) {
				log_print("LIB_SYS not acquired\n");
				return 0;
			}
			rplHandle = sysapp_handle;
			break;

		case LIB_VPAD:
			if (DEBUG_LOG_DYN)log_printf("FindExport of %s! From LIB_VPAD\n", functionName);
			if (vpad_handle == 0) {
				log_print("LIB_VPAD not acquired\n");
				return 0;
			}
			rplHandle = vpad_handle;
			break;

		case LIB_NN_ACP:
			if (DEBUG_LOG_DYN)log_printf("FindExport of %s! From LIB_NN_ACP\n", functionName);
			if (acp_handle == 0) {
				log_print("LIB_NN_ACP not acquired\n");
				return 0;
			}
			rplHandle = acp_handle;
			break;

		case LIB_SYSHID:
			if (DEBUG_LOG_DYN)log_printf("FindExport of %s! From LIB_SYSHID\n", functionName);
			if (syshid_handle == 0) {
				log_print("LIB_SYSHID not acquired\n");
				return 0;
			}
			rplHandle = syshid_handle;
			break;

		case LIB_VPADBASE:
			if (DEBUG_LOG_DYN)log_printf("FindExport of %s! From LIB_VPADBASE\n", functionName);
			if (vpadbase_handle == 0) {
				log_print("LIB_VPADBASE not acquired\n");
				return 0;
			}
			rplHandle = vpadbase_handle;
			break; */

		default:;
			char messageBuffer[50];
			__os_snprintf(messageBuffer, 50, "Unhandled library %i", library);
			OSFatal(messageBuffer);
			break;
	}

	return rplHandle;
}

unsigned int getFunctionAddress(unsigned int library, const char *functionName) {
	unsigned int realAddress = 0;

	if (strcmp(functionName, "OSDynLoad_Acquire") == 0) {
		memcpy(&realAddress, &OSDynLoad_Acquire, 4);
		return realAddress;
	} else if (strcmp(functionName, "LiWaitOneChunk") == 0) {
		realAddress = (unsigned int) addr_LiWaitOneChunk;
		return realAddress;
	} else if (strcmp(functionName, "LiBounceOneChunk") == 0) {
		//! not required on firmwares above 3.1.0
		if (OS_FIRMWARE >= 400)
			return 0;

		unsigned int addr_LiBounceOneChunk = 0x010003A0;
		realAddress = addr_LiBounceOneChunk;
		return realAddress;
	}

	unsigned int rpl_handle = getRPLHandle(library, functionName);

	if (!rpl_handle) {
		log_printf("Failed to find the RPL handle for %s\n", functionName);
		return 0;
	}

	OSDynLoad_FindExport((u32) rpl_handle, 0, functionName, &realAddress);

	if (!realAddress) {
		log_printf("[Get] OSDynLoad_FindExport failed for %s\n", functionName);
		return 0;
	}

	if ((u32) (*(volatile unsigned int *) (realAddress) & 0x48000002) == 0x48000000) {
		unsigned int address_diff = (u32) (*(volatile unsigned int *) (realAddress) & 0x03FFFFFC);
		if ((address_diff & 0x03000000) == 0x03000000) {
			address_diff |= 0xFC000000;
		}
		realAddress += (int) address_diff;
		if ((u32) (*(volatile unsigned int *) (realAddress) & 0x48000002) == 0x48000000) {
			return 0;
		}
	}

	return realAddress;
}