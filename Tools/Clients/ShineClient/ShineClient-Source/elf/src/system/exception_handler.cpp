#include <stdio.h>
#include "../dynamic_libs/os_functions.h"
#include "../utils/logger.h"
#include "exception_handler.h"

static const char *exception_names[] = {
		"DSI",
		"ISI",
		"PROGRAM"
};

static const char exception_print_formats[18][45] = {
		"Exception type %s occurred!\n",                       // 0
		"GPR00 %08X GPR08 %08X GPR16 %08X GPR24 %08X\n",       // 1
		"GPR01 %08X GPR09 %08X GPR17 %08X GPR25 %08X\n",       // 2
		"GPR02 %08X GPR10 %08X GPR18 %08X GPR26 %08X\n",       // 3
		"GPR03 %08X GPR11 %08X GPR19 %08X GPR27 %08X\n",       // 4
		"GPR04 %08X GPR12 %08X GPR20 %08X GPR28 %08X\n",       // 5
		"GPR05 %08X GPR13 %08X GPR21 %08X GPR29 %08X\n",       // 6
		"GPR06 %08X GPR14 %08X GPR22 %08X GPR30 %08X\n",       // 7
		"GPR07 %08X GPR15 %08X GPR23 %08X GPR31 %08X\n",       // 8
		"LR    %08X SRR0  %08x SRR1  %08x\n",                  // 9
		"DAR   %08X DSISR %08X\n",                             // 10
		"\nSTACK DUMP:",                                       // 11
		" --> ",                                               // 12
		" -->\n",                                             // 13
		"\n",                                                 // 14
		"%p",                                                 // 15
		"\nCODE DUMP:\n",                                     // 16
		"%p:  %08X %08X %08X %08X\n",                         // 17
};

static unsigned char exceptionCallback(void *c, unsigned char exception_type) {
	char stackTraceBuffer[850];
	int pos = 0;

	OSContext *context = (OSContext *) c;
	/*
	 * This part is mostly from libogc. Thanks to the devs over there.
	 */
	pos += sprintf(stackTraceBuffer + pos, exception_print_formats[0], exception_names[exception_type]);
	pos += sprintf(stackTraceBuffer + pos, exception_print_formats[1], context->gpr[0], context->gpr[8],
				   context->gpr[16],
				   context->gpr[24]);
	pos += sprintf(stackTraceBuffer + pos, exception_print_formats[2], context->gpr[1], context->gpr[9],
				   context->gpr[17],
				   context->gpr[25]);
	pos += sprintf(stackTraceBuffer + pos, exception_print_formats[3], context->gpr[2], context->gpr[10],
				   context->gpr[18],
				   context->gpr[26]);
	pos += sprintf(stackTraceBuffer + pos, exception_print_formats[4], context->gpr[3], context->gpr[11],
				   context->gpr[19],
				   context->gpr[27]);
	pos += sprintf(stackTraceBuffer + pos, exception_print_formats[5], context->gpr[4], context->gpr[12],
				   context->gpr[20],
				   context->gpr[28]);
	pos += sprintf(stackTraceBuffer + pos, exception_print_formats[6], context->gpr[5], context->gpr[13],
				   context->gpr[21],
				   context->gpr[29]);
	pos += sprintf(stackTraceBuffer + pos, exception_print_formats[7], context->gpr[6], context->gpr[14],
				   context->gpr[22],
				   context->gpr[30]);
	pos += sprintf(stackTraceBuffer + pos, exception_print_formats[8], context->gpr[7], context->gpr[15],
				   context->gpr[23],
				   context->gpr[31]);
	pos += sprintf(stackTraceBuffer + pos, exception_print_formats[9], context->lr, context->srr0, context->srr1);

	//if(exception_type == OS_EXCEPTION_DSI) {
	pos += sprintf(stackTraceBuffer + pos, exception_print_formats[10], context->exception_specific1,
				   context->exception_specific0); // this freezes
	//}

	void *pc = (void *) context->srr0;
	void *lr = (void *) context->lr;
	void *r1 = (void *) context->gpr[1];
	register uint32_t currentStackTraceDepth = 0;
	register frame_rec_t l, p = (frame_rec_t) lr;

	l = p;
	p = (frame_rec_t) r1;
	if (!p)
			asm volatile("mr %0,%%r1" : "=r"(p));

	pos += sprintf(stackTraceBuffer + pos, exception_print_formats[11]);

	for (currentStackTraceDepth = 0;
		 currentStackTraceDepth < CPU_STACK_TRACE_DEPTH - 1 && p->up; p = p->up, currentStackTraceDepth++) {
		if (currentStackTraceDepth % 4)
			pos += sprintf(stackTraceBuffer + pos, exception_print_formats[12]);
		else {
			if (currentStackTraceDepth > 0)
				pos += sprintf(stackTraceBuffer + pos, exception_print_formats[13]);
			else
				pos += sprintf(stackTraceBuffer + pos, exception_print_formats[14]);
		}

		switch (currentStackTraceDepth) {
			case 0:
				if (pc)
					pos += sprintf(stackTraceBuffer + pos, exception_print_formats[15], pc);
				break;
			case 1:
				if (!l)
					l = (frame_rec_t) mfspr(8);
				pos += sprintf(stackTraceBuffer + pos, exception_print_formats[15], (void *) l);
				break;
			default:
				pos += sprintf(stackTraceBuffer + pos, exception_print_formats[15], (void *) (p->up->lr));
				break;
		}
	}

	//if(exception_type == OS_EXCEPTION_DSI) {
	uint32_t *pAdd = (uint32_t *) context->srr0;
	pos += sprintf(stackTraceBuffer + pos, exception_print_formats[16]);
	// TODO by Dimok: this was actually be 3 instead of 2 lines in libogc .... but there is just no more space anymore on the screen
	for (currentStackTraceDepth = 0; currentStackTraceDepth < 8; currentStackTraceDepth += 4)
		pos += sprintf(stackTraceBuffer + pos, exception_print_formats[17], &(pAdd[currentStackTraceDepth]),
					   pAdd[currentStackTraceDepth], pAdd[currentStackTraceDepth + 1], pAdd[currentStackTraceDepth + 2],
					   pAdd[currentStackTraceDepth + 3]);
	//}
	log_print(stackTraceBuffer);
	OSFatal(stackTraceBuffer);

	return 1;
}

unsigned char dsi_exception_cb(void *context) {
	return exceptionCallback(context, 0);
}

unsigned char isi_exception_cb(void *context) {
	return exceptionCallback(context, 1);
}

unsigned char program_exception_cb(void *context) {
	return exceptionCallback(context, 2);
}

void setup_os_exceptions() {
	OSSetExceptionCallback(OS_EXCEPTION_DSI, &dsi_exception_cb);
	OSSetExceptionCallback(OS_EXCEPTION_ISI, &isi_exception_cb);
	OSSetExceptionCallback(OS_EXCEPTION_PROGRAM, &program_exception_cb);
}