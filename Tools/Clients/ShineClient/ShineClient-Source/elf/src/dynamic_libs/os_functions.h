/****************************************************************************
 * Copyright (C) 2015
 * by Dimok
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you
 * must not claim that you wrote the original software. If you use
 * this software in a product, an acknowledgment in the product
 * documentation would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 ***************************************************************************/
#ifndef __OS_FUNCTIONS_H_
#define __OS_FUNCTIONS_H_

#include <gctypes.h>
#include "common/os_defs.h"
// #include "../cafe.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Disassembler */
typedef void (*DisasmReport)(char *outputBuffer, ...);

typedef void *(*DisasmGetSym)(u32 addr, u8 *symbolName, u32 nameBufSize);

#define PPC_DISASM_MAX_BUFFER 64

#define PPC_DISASM_DEFAULT     0x00000000  // use defaults
#define PPC_DISASM_SIMPLIFY    0x00000001  // use simplified mnemonics
#define PPC_DISASM_REG_SPACES  0x00000020  // emit spaces between registers
#define PPC_DISASM_EMIT_DISASM 0x00000040  // emit only disassembly
#define PPC_DISASM_EMIT_ADDR   0x00000080  // emit only addresses + disassembly
#define PPC_DISASM_EMIT_FUNCS  0x00000100  // emit function names before and during disassembly

/* zlib */

/*#define Z_NO_COMPRESSION         0
#define Z_BEST_SPEED             1
#define Z_BEST_COMPRESSION       9
#define Z_DEFAULT_COMPRESSION  (-1)

#define Z_OK            0
#define Z_STREAM_END    1
#define Z_NEED_DICT     2
#define Z_ERRNO        (-1)
#define Z_STREAM_ERROR (-2)
#define Z_DATA_ERROR   (-3)
#define Z_MEM_ERROR    (-4)
#define Z_BUF_ERROR    (-5)
#define Z_VERSION_ERROR (-6)*/

#define BUS_SPEED                       248625000
#define SECS_TO_TICKS(sec)              (((unsigned long long)(sec)) * (BUS_SPEED/4))
#define MILLISECS_TO_TICKS(msec)        (SECS_TO_TICKS(msec) / 1000)
#define MICROSECS_TO_TICKS(usec)        (SECS_TO_TICKS(usec) / 1000000)

#define usleep(usecs)                   OSSleepTicks(MICROSECS_TO_TICKS(usecs))
#define sleep(secs)                     OSSleepTicks(SECS_TO_TICKS(secs))

#define FLUSH_DATA_BLOCK(addr)          asm volatile("dcbf 0, %0; sync" : : "r"(((addr) & ~31)))
#define INVAL_DATA_BLOCK(addr)          asm volatile("dcbi 0, %0; sync" : : "r"(((addr) & ~31)))

#define EXPORT_DECL(res, func, ...)     res (* func)(__VA_ARGS__) __attribute__((section(".data"))) = 0;
#define EXPORT_VAR(type, var)           type var __attribute__((section(".data")));


#define EXPORT_FUNC_WRITE(func, val)    *(u32*)(((u32)&func) + 0) = (u32)val

#define OS_FIND_EXPORT(handle, func)    funcPointer = 0;                                                                \
                                        OSDynLoad_FindExport(handle, 0, # func, &funcPointer);                          \
                                        if(!funcPointer)                                                                \
                                            OSFatal("Function " # func " is NULL");                                     \
                                        EXPORT_FUNC_WRITE(func, funcPointer);

#define OS_FIND_EXPORT_EX(handle, func, func_p)                                                                         \
                                        funcPointer = 0;                                                                \
                                        OSDynLoad_FindExport(handle, 0, # func, &funcPointer);                          \
                                        if(!funcPointer)                                                                \
                                            OSFatal("Function " # func " is NULL");                                     \
                                        EXPORT_FUNC_WRITE(func_p, funcPointer);

#define OS_MUTEX_SIZE                   44

/* Handle for coreinit */
extern u32 coreinit_handle;

void InitOSFunctionPointers(void);

void InitAcquireOS(void);

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! Lib handle functions
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern int (*OSDynLoad_Acquire)(const char *rpl, u32 *handle);

extern int (*OSDynLoad_FindExport)(u32 handle, int isdata, const char *symbol, void *address);

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! Security functions
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern int (*OSGetSecurityLevel)(void);

extern void (*__OSSetCoreTrace)(int core);

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! Thread functions
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//
// Thread attributes
//
// Threads lacking an affinity bit are affinitized to the
// core on which they are created
//

#define OS_THREAD_ATTR_AFFINITY_NONE    0x0007u        // affinity to run on every core
#define OS_THREAD_ATTR_AFFINITY_CORE0   0x0001u        // run only on core0
#define OS_THREAD_ATTR_AFFINITY_CORE1   0x0002u        // run only on core1
#define OS_THREAD_ATTR_AFFINITY_CORE2   0x0004u        // run only on core2
#define OS_THREAD_ATTR_DETACH           0x0008u        // detached
#define OS_THREAD_ATTR_PINNED_AFFINITY  0x0010u        // pinned (affinitized) to a single core
#define OS_THREAD_ATTR_CHECK_STACK_USE  0x0040u        // check for stack usage
#define OS_THREAD_ATTR_NAME_SENT        0x0080u        // debugger has seen the name
#define OS_THREAD_ATTR_LAST             (OS_THREAD_ATTR_DETACH | OS_THREAD_ATTR_PINNED_AFFINITY | OS_THREAD_ATTR_AFFINITY_NONE)

extern int
(*OSCreateThread)(void *thread, s32 (*callback)(s32, void *), s32 argc, void *args, u32 stack, u32 stack_size,
				  s32 priority, u32 attr);

extern void (*OSEnableInterrupts)(void);

extern void (*__OSClearAndEnableInterrupt)(void);

extern int (*OSIsInterruptEnabled)(void);

extern int (*OSIsDebuggerPresent)(void);

// extern bool (*OSSendMessage)(struct OSMessageQueue*, struct OSMessage*, s32);

extern void (*OSRestoreInterrupts)(void);

extern void (*OSSetDABR)(int, int, int, int);

extern void (*OSSetIABR)(int, int);

extern int (*OSGetCurrentThread)(void);

extern int (*OSResumeThread)(void *thread);

extern int (*OSSleepThread)(void *thread);

extern int (*OSWakeupThread)(void *thread);

extern void (*OSSetThreadName)(void *thread, const char *name);

extern int (*OSSuspendThread)(void *thread);

extern int (*OSIsThreadTerminated)(void *thread);

extern int (*OSIsThreadSuspended)(void *thread);

extern int (*OSJoinThread)(void *thread, int *ret_val);

extern int (*OSSetThreadPriority)(void *thread, int priority);

extern void (*OSDetachThread)(void *thread);

extern void (*OSSleepTicks)(u64 ticks);

extern u64 (*OSGetTick)(void);

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! Mutex functions
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern void (*OSInitMutex)(void *mutex);

extern void (*OSLockMutex)(void *mutex);

extern void (*OSUnlockMutex)(void *mutex);

extern int (*OSTryLockMutex)(void *mutex);

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! System functions
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern u64 (*OSGetTitleID)(void);

extern void (*__Exit)(void);

extern void (*OSFatal)(const char *msg);

extern void (*DCFlushRange)(const void *addr, u32 length);

extern void (*ICInvalidateRange)(const void *addr, u32 length);

extern void *(*OSEffectiveToPhysical)(const void *);

extern int (*OSIsAddressValid)(const void *);

extern int (*__OSValidateAddressSpaceRange)(int dummyTrue, void *address, int size);

extern int (*__os_snprintf)(char *buffer, int length, const char *format, ...);

extern int *(*__gh_errno_ptr)(void);

extern void (*OSScreenInit)(void);

extern unsigned int (*OSScreenGetBufferSizeEx)(unsigned int bufferNum);

extern int (*OSScreenSetBufferEx)(unsigned int bufferNum, void *addr);

extern int (*OSScreenClearBufferEx)(unsigned int bufferNum, unsigned int temp);

extern int (*OSScreenFlipBuffersEx)(unsigned int bufferNum);

extern int (*OSScreenPutFontEx)(unsigned int bufferNum, unsigned int posX, unsigned int posY, const char *buffer);

extern int (*OSScreenEnableEx)(unsigned int bufferNum, int enable);

typedef unsigned char (*exception_callback)(void *interruptedContext);

extern void (*OSSetExceptionCallback)(u8 exceptionType, exception_callback callback);

extern void (*OSSetExceptionCallbackEx)(u8 exceptionMode, u8 exceptionType, exception_callback callback);

extern void (*__OSSetInterruptHandler)(u8 interruptType, exception_callback callback);

extern int (*OSAllocFromSystem)(unsigned int size, unsigned int align);

extern int (*OSFreeToSystem)(void *aPtr);

extern void
(*DisassemblePPCRange)(void *rangeStart, void *rangeEnd, DisasmReport disasmReport, DisasmGetSym disasmGetSym,
					   u32 disasmOptions);

extern bool
(*DisassemblePPCOpcode)(u32 *opcode, char *outputBuffer, u32 bufferSize, DisasmGetSym disasmGetSym, u32 disasmOptions);

extern void *(*OSGetSymbolName)(u32 addr, u8 *symbolName, u32 nameBufSize);

extern int (*OSIsDebuggerInitialized)(void);

// extern int (*compress2)(char *dest, int *destLen, const char *source, int sourceLen, int level);

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! MCP functions
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern int (*MCP_Open)(void);

extern int (*MCP_Close)(int handle);

extern int (*MCP_GetOwnTitleInfo)(int handle, void *data);

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! LOADER functions
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern int (*LiWaitIopComplete)(int unknown_syscall_arg_r3, int *remaining_bytes);

extern int (*LiWaitIopCompleteWithInterrupts)(int unknown_syscall_arg_r3, int *remaining_bytes);

extern void (*addr_LiWaitOneChunk)(void);

extern void (*addr_sgIsLoadingBuffer)(void);

extern void (*addr_gDynloadInitialized)(void);

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! Kernel function addresses
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern void (*addr_PrepareTitle_hook)(void);

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! Other function addresses
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern void (*DCInvalidateRange)(void *buffer, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif // __OS_FUNCTIONS_H_

