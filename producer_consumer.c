#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define ITEMS_TO_PRODUCE 20

int buffer;               // single-slot buffer
sem_t semEmpty;           // 1 when buffer is empty
sem_t semFull;            // 1 when buffer is full

void* producer_thread(void* arg) {
	(void)arg;
	for (int i = 1; i <= ITEMS_TO_PRODUCE; ++i) {
		sem_wait(&semEmpty);
		buffer = i;          // write item
		printf("Produced %d\n", i);
		sem_post(&semFull);
	}
	return NULL;
}

void* consumer_thread(void* arg) {
	(void)arg;
	for (int i = 1; i <= ITEMS_TO_PRODUCE; ++i) {
		sem_wait(&semFull);
		int item = buffer;   // read item
		printf("Consumed %d\n", item);
		sem_post(&semEmpty);
	}
	return NULL;
}

int main(void) {
	// empty starts as 1 (slot available), full starts as 0 (no item yet)
	if (sem_init(&semEmpty, 0, 1) != 0) {
		perror("sem_init semEmpty");
		return 1;
	}
	if (sem_init(&semFull, 0, 0) != 0) {
		perror("sem_init semFull");
		return 1;
	}

	pthread_t producer;
	pthread_t consumer;

	if (pthread_create(&producer, NULL, producer_thread, NULL) != 0) {
		perror("pthread_create producer");
		return 1;
	}
	if (pthread_create(&consumer, NULL, consumer_thread, NULL) != 0) {
		perror("pthread_create consumer");
		return 1;
	}

	pthread_join(producer, NULL);
	pthread_join(consumer, NULL);

	sem_destroy(&semEmpty);
	sem_destroy(&semFull);

	printf("Done.\n");
	return 0;
}


