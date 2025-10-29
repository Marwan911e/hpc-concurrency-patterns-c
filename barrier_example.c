#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#define NUM_THREADS 4

pthread_barrier_t barrier;

void* worker(void* arg) {
	long id = (long)(intptr_t)arg;
	// Phase 1: initialization (placeholder)
	printf("Thread %ld: init done\n", id);

	// Wait for all threads at the barrier
	int rc = pthread_barrier_wait(&barrier);
	if (rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD) {
		perror("pthread_barrier_wait");
		return NULL;
	}

	// Phase 2: main computation starts only after everyone reached the barrier
	printf("Thread %ld: main work starts\n", id);
	return NULL;
}

int main(void) {
	pthread_t threads[NUM_THREADS];

	if (pthread_barrier_init(&barrier, NULL, NUM_THREADS) != 0) {
		perror("pthread_barrier_init");
		return 1;
	}

	for (long i = 0; i < NUM_THREADS; ++i) {
		if (pthread_create(&threads[i], NULL, worker, (void*)(intptr_t)i) != 0) {
			perror("pthread_create");
			return 1;
		}
	}

	for (int i = 0; i < NUM_THREADS; ++i) {
		pthread_join(threads[i], NULL);
	}

	pthread_barrier_destroy(&barrier);
	printf("Done.\n");
	return 0;
}


