#include "../utils/function_patcher.h"
#include "../utils/logger.h"

declareFunctionHook(int, FSOpenFile, void *pClient, void *pCmd, const char *path, const char *mode, int *fd, int errHandling) {
	log_printf("FSOpenFile(): %s", path);
	return real_FSOpenFile(pClient, pCmd, path, mode, fd, errHandling);
}

declareFunctionHook(int, FSOpenFileAsync, void *pClient, void *pCmd, const char *path, const char *mode, int *handle, int error, const void *asyncParams) {
	log_printf("FSOpenFileAsync(): %s", path);
	return real_FSOpenFileAsync(pClient, pCmd, path, mode, handle, error, asyncParams);
}

declareFunctionHook(int, FSOpenDir, void *pClient, void *pCmd, const char *path, s32 *dh, int errHandling) {
	log_printf("FSOpenDir(): %s", path);
	return real_FSOpenDir(pClient, pCmd, path, dh, errHandling);
}

declareFunctionHook(int, FSOpenDirAsync, void *pClient, void *pCmd, const char *path, int *handle, int error, void *asyncParams) {
	log_printf("FSOpenDirAsync(): %s", path);
	return real_FSOpenDirAsync(pClient, pCmd, path, handle, error, asyncParams);
}

declareFunctionHook(int, FSGetStat, void *pClient, void *pCmd, const char *path, FSStat *stats, int errHandling) {
	log_printf("FSGetStat(): %s", path);
	return real_FSGetStat(pClient, pCmd, path, stats, errHandling);
}

declareFunctionHook(int, FSGetStatAsync, void *pClient, void *pCmd, const char *path, FSStat *stats, int errHandling, void *asyncParams) {
	log_printf("FSGetStatAsync(): %s", path);
	return real_FSGetStatAsync(pClient, pCmd, path, stats, errHandling, asyncParams);
}

FunctionHook method_hooks_coreinit[] __attribute__((section(".data"))) = {
		makeFunctionHook(FSOpenFile, LIB_CORE_INIT, STATIC_FUNCTION),
		makeFunctionHook(FSOpenFileAsync, LIB_CORE_INIT, STATIC_FUNCTION),

		makeFunctionHook(FSOpenDir, LIB_CORE_INIT, STATIC_FUNCTION),
		makeFunctionHook(FSOpenDirAsync, LIB_CORE_INIT, STATIC_FUNCTION),

		makeFunctionHook(FSGetStat, LIB_CORE_INIT, STATIC_FUNCTION),
		makeFunctionHook(FSGetStatAsync, LIB_CORE_INIT, STATIC_FUNCTION),
};

u32 method_hooks_size_coreinit __attribute__((section(".data"))) = sizeof(method_hooks_coreinit) / sizeof(FunctionHook);

volatile unsigned int method_calls_coreinit[sizeof(method_hooks_coreinit) / sizeof(FunctionHook) * FUNCTION_PATCHER_METHOD_STORE_SIZE] __attribute__((section(".data")));