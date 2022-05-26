#ifndef TCPGECKO_UTILITIES_H
#define TCPGECKO_UTILITIES_H

#include "../dynamic_libs/os_functions.h"
#include "../utils/logger.h"
#include "kernel.h"
#include <stddef.h> /* size_t */

void writeCode(u32 address, u32 instruction) {

	u32 *pointer = (u32 *) (address + 0xA0000000);
	*pointer = instruction;
	DCFlushRange(pointer, 4);
	ICInvalidateRange(pointer, 4);

}

void patchFunction(char *function, char *patchBytes, unsigned int patchBytesSize, int functionOffset) {
	log_print("Patching function...\n");
	void *patchAddress = function + functionOffset;
	log_printf("Patch address: %p\n", patchAddress);
	kernelCopyInt((unsigned char *) patchAddress, (unsigned char *) patchBytes, patchBytesSize);
	log_print("Successfully patched!\n");
}

int memoryCompare(const void *s1, const void *s2, size_t n) {
	const unsigned char *p1 = static_cast<const unsigned char *>(s1);
	const unsigned char *p2 = static_cast<const unsigned char *>(s2);
	while (n--)
		if (*p1 != *p2)
			return *p1 - *p2;
		else
			p1++, p2++;
	return 0;
}

#endif