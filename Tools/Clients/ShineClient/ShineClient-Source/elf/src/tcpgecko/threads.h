#ifndef TCPGECKO_THREADS_H
#define TCPGECKO_THREADS_H

#define THREAD_SIZE 0x6A0
#define PREVIOUS_THREAD 0x390
#define NEXT_THREAD 0x38C

struct node *getAllThreads();

#endif