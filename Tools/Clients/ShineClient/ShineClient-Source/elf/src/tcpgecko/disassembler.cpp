#include "../dynamic_libs/os_functions.h"
#include "assertions.h"
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

char *disassemblerBuffer;
void *disassemblerBufferPointer;

#define DISASSEMBLER_BUFFER_SIZE 0x1024

void formatDisassembled(char *format, ...) {
	if (!disassemblerBuffer) {
		disassemblerBuffer = (char *) malloc(DISASSEMBLER_BUFFER_SIZE);
		ASSERT_ALLOCATED(disassemblerBuffer, "Disassembler buffer")
		disassemblerBufferPointer = disassemblerBuffer;
	}

	va_list variableArguments;
	va_start(variableArguments, format);
	char *temporaryBuffer;
	int printedBytesCount = vasprintf(&temporaryBuffer, format, variableArguments);
	ASSERT_ALLOCATED(temporaryBuffer, "Temporary buffer")
	ASSERT_MINIMUM_HOLDS(printedBytesCount, 1, "Printed bytes count")
	va_end(variableArguments);

	// Do not smash the buffer
	long projectedSize = disassemblerBuffer - (char *) disassemblerBufferPointer + printedBytesCount;
	if (projectedSize < DISASSEMBLER_BUFFER_SIZE) {
		memcpy(disassemblerBuffer, temporaryBuffer, printedBytesCount);
		disassemblerBuffer += printedBytesCount;
	}

	free(temporaryBuffer);
}