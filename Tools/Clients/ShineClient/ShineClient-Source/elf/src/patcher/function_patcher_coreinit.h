#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../utils/function_patcher.h"

extern FunctionHook method_hooks_coreinit[];
extern u32 method_hooks_size_coreinit;
extern volatile unsigned int method_calls_coreinit[];

#ifdef __cplusplus
}
#endif