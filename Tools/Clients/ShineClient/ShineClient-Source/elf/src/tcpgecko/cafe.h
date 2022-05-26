#ifndef TCPGECKO_STACK_H
#define TCPGECKO_STACK_H

typedef struct OSThread;

typedef struct OSThreadLink {
	OSThread *next;
	OSThread *prev;
} OSThreadLink;

typedef struct OSThreadQueue {
	OSThread *head;
	OSThread *tail;
	void *parentStruct;
	u32 reserved;
} OSThreadQueue;

typedef struct OSMessage {
	u32 message;
	u32 data0;
	u32 data1;
	u32 data2;
} OSMessage;

typedef struct OSMessageQueue {
	u32 tag;
	char *name;
	u32 reserved;

	OSThreadQueue sendQueue;
	OSThreadQueue recvQueue;
	OSMessage *messages;
	int msgCount;
	int firstIndex;
	int usedCount;
} OSMessageQueue;

typedef struct OSContext {
	char tag[8];

	u32 gpr[32];

	u32 cr;
	u32 lr;
	u32 ctr;
	u32 xer;

	u32 srr0;
	u32 srr1;

	u32 ex0;
	u32 ex1;

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

typedef int (*ThreadFunc)(int argc, void *argv);

typedef struct OSThread {
	OSContext context;

	u32 txtTag;
	u8 state;
	u8 attr;

	short threadId;
	int suspend;
	int priority;

	char _[0x394 - 0x330];

	void *stackBase;
	void *stackEnd;

	ThreadFunc entryPoint;

	char _3A0[0x6A0 - 0x3A0];
} OSThread;

#endif