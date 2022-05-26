#ifndef _MAIN_H_
#define _MAIN_H_

#include "../common/types.h"
#include "../dynamic_libs/os_functions.h"

#ifdef __cplusplus
extern "C" {
#endif

int Menu_Main();

extern bool isCodeHandlerInstalled;
extern bool areSDCheatsEnabled;

#ifdef __cplusplus
}
#endif

#endif