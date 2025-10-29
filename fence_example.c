#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>

// Demonstrates memory ordering with C11 atomics: release -> acquire
// One writer publishes data then flips a flag with release semantics.
// One reader spins on the flag with acquire semantics, then reads data safely.

int data = 0;                 // normal shared data
atomic_int readyFlag = 0;     // publication flag

void* writer(void* arg) {
	(void)arg;
	data = 42; // write the payload first
	// Publish: any writes before this point become visible to threads
	// that subsequently observe readyFlag with acquire
	atomic_store_explicit(&readyFlag, 1, memory_order_release);
	return NULL;
}

void* reader(void* arg) {
	(void)arg;
	// Wait until writer publishes
	while (atomic_load_explicit(&readyFlag, memory_order_acquire) == 0) {
		// spin (short demo)
	}
	printf("Reader saw data = %d\n", data); // guaranteed to be 42
	return NULL;
}

int main(void) {
	pthread_t tWriter;
	pthread_t tReader;

	if (pthread_create(&tReader, NULL, reader, NULL) != 0) {
		perror("pthread_create reader");
		return 1;
	}
	if (pthread_create(&tWriter, NULL, writer, NULL) != 0) {
		perror("pthread_create writer");
		return 1;
	}

	pthread_join(tWriter, NULL);
	pthread_join(tReader, NULL);

	printf("Done.\n");
	return 0;
}


