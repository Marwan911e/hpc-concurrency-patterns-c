#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define INCREMENTS_PER_THREAD 1000000

long long counter = 0;               // shared counter
pthread_mutex_t counterMutex = PTHREAD_MUTEX_INITIALIZER; // lock protecting counter

void* worker(void* arg) {
	(void)arg;
	for (int i = 0; i < INCREMENTS_PER_THREAD; ++i) {
		pthread_mutex_lock(&counterMutex);
		counter++;
		pthread_mutex_unlock(&counterMutex);
	}
	return NULL;
}

int main(void) {
	pthread_t threads[NUM_THREADS];

	for (int i = 0; i < NUM_THREADS; ++i) {
		if (pthread_create(&threads[i], NULL, worker, NULL) != 0) {
			perror("pthread_create");
			return 1;
		}
	}

	for (int i = 0; i < NUM_THREADS; ++i) {
		pthread_join(threads[i], NULL);
	}

	long long expected = (long long)NUM_THREADS * (long long)INCREMENTS_PER_THREAD;
	printf("Expected: %lld, Actual: %lld\n", expected, counter);

	pthread_mutex_destroy(&counterMutex);
	return 0;
}


