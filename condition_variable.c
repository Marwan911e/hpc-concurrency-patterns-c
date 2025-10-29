#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int ready = 0; // shared condition/predicate
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

void* waiter(void* arg) {
	(void)arg;
	pthread_mutex_lock(&m);
	while (!ready) {
		pthread_cond_wait(&cv, &m); // atomically unlocks m and waits, then re-locks
	}
	printf("Waiter: condition is true, proceeding.\n");
	pthread_mutex_unlock(&m);
	return NULL;
}

void* signaler(void* arg) {
	(void)arg;
	// Do some work, then set condition and signal
	pthread_mutex_lock(&m);
	ready = 1;
	printf("Signaler: set condition, signaling.\n");
	pthread_cond_signal(&cv);
	pthread_mutex_unlock(&m);
	return NULL;
}

int main(void) {
	pthread_t tWaiter;
	pthread_t tSignaler;

	if (pthread_create(&tWaiter, NULL, waiter, NULL) != 0) {
		perror("pthread_create waiter");
		return 1;
	}
	if (pthread_create(&tSignaler, NULL, signaler, NULL) != 0) {
		perror("pthread_create signaler");
		return 1;
	}

	pthread_join(tWaiter, NULL);
	pthread_join(tSignaler, NULL);

	pthread_mutex_destroy(&m);
	pthread_cond_destroy(&cv);
	printf("Done.\n");
	return 0;
}


