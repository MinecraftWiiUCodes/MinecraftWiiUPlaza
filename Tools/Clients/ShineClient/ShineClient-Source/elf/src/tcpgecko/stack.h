#ifndef TCPGECKO_STACK_H
#define TCPGECKO_STACK_H

#include "linked_list.h"
#include "cafe.h"

bool isValidStackPointer(u32 stackPointer) {
	return stackPointer >= 0x10000000 && stackPointer < 0x20000000;
}

struct node *getStackTrace(OSContext *context) {
	struct node *stackTrace = NULL;
	u32 stackPointer = context->gpr[1];
	u32 stackPointersCount = 0;

	while (isValidStackPointer(stackPointer)) {
		stackPointer = *(u32 *) stackPointer;
		if (!isValidStackPointer(stackPointer)) {
			break;
		}

		int data = *(u32 * )(stackPointer + 4);
		stackTrace = insert(stackTrace, (void *) data);
		stackPointersCount++;
	}

	return stackTrace;
}

#endif
