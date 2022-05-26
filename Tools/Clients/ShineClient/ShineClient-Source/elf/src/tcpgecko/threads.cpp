#include "threads.h"
#include "linked_list.h"
#include "../dynamic_libs/os_functions.h"
#include "../utils/logger.h"

struct node *getAllThreads() {
	struct node *threads = NULL;
	int currentThreadAddress = OSGetCurrentThread();
	log_printf("Thread address: %08x\n", currentThreadAddress);
	int iterationThreadAddress = currentThreadAddress;
	int temporaryThreadAddress;

	// Follow "previous thread" pointers back to the beginning
	while ((temporaryThreadAddress = *(int *) (iterationThreadAddress + PREVIOUS_THREAD)) != 0) {
		log_printf("Temporary thread address going backwards: %08x\n", temporaryThreadAddress);
		iterationThreadAddress = temporaryThreadAddress;
	}

	// Now iterate over all threads
	while ((temporaryThreadAddress = *(int *) (iterationThreadAddress + NEXT_THREAD)) != 0) {
		// Grab the thread's address
		log_printf("Temporary thread address going forward: %08x\n", temporaryThreadAddress);
		threads = insert(threads, (void *) iterationThreadAddress);
		log_printf("Inserted: %08x\n", iterationThreadAddress);
		iterationThreadAddress = temporaryThreadAddress;
	}

	// The previous while would skip the last thread so add it as well
	threads = insert(threads, (void *) iterationThreadAddress);
	log_printf("Inserted: %08x\n", iterationThreadAddress);

	reverse(&threads);

	return threads;
}