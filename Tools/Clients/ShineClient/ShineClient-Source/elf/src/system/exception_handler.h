#ifndef __EXCEPTION_HANDLER_H_
#define __EXCEPTION_HANDLER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../tcpgecko/stringify.h"

#define OS_EXCEPTION_DSI                        2
#define OS_EXCEPTION_ISI                        3
#define OS_EXCEPTION_PROGRAM                    6

/* Exceptions */
typedef struct OSContext {
	/* OSContext identifier */
	uint32_t tag1;
	uint32_t tag2;

	/* GPRs */
	uint32_t gpr[32];

	/* Special registers */
	uint32_t cr;
	uint32_t lr;
	uint32_t ctr;
	uint32_t xer;

	/* Initial PC and MSR */
	uint32_t srr0;
	uint32_t srr1;

	/* Only valid during DSI exception */
	uint32_t exception_specific0;
	uint32_t exception_specific1;

	u32 exception_type;
	u32 reserved;

	double fpscr;
	double fpr[32];

	u16 spinLockCount;
	u16 state;

	u32 gqr[8];
	u32 pir;
	double psf[32];

	u64 coretime[3];
	u64 starttime;

	u32 error;
	u32 attributes;

	u32 pmc1;
	u32 pmc2;
	u32 pmc3;
	u32 pmc4;
	u32 mmcr0;
	u32 mmcr1;
} OSContext;

#define CPU_STACK_TRACE_DEPTH        10

// http://elixir.free-electrons.com/linux/v2.6.24/source/include/asm-powerpc/reg.h#L713
#define mfspr(spr) \
({    register uint32_t _rval = 0; \
    asm volatile("mfspr %0," __stringify(spr) \
    : "=r" (_rval));\
    _rval; \
})

typedef struct _framerec {
	struct _framerec *up;
	void *lr;
} frame_rec, *frame_rec_t;

void setup_os_exceptions(void);

unsigned char dsi_exception_cb(void *context);
unsigned char isi_exception_cb(void *context);
unsigned char program_exception_cb(void *context);

#ifdef __cplusplus
}
#endif

#endif
