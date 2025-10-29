CC=gcc
CFLAGS=-Wall -Wextra -O2 -std=c11
LDFLAGS=-pthread

TARGETS=producer_consumer counter_mutex condition_variable fence_example barrier_example

all: $(TARGETS)

producer_consumer: producer_consumer.c
	$(CC) $(CFLAGS) producer_consumer.c -o producer_consumer $(LDFLAGS)

counter_mutex: counter_mutex.c
	$(CC) $(CFLAGS) counter_mutex.c -o counter_mutex $(LDFLAGS)

condition_variable: condition_variable.c
	$(CC) $(CFLAGS) condition_variable.c -o condition_variable $(LDFLAGS)

fence_example: fence_example.c
	$(CC) $(CFLAGS) fence_example.c -o fence_example $(LDFLAGS)

barrier_example: barrier_example.c
	$(CC) $(CFLAGS) barrier_example.c -o barrier_example $(LDFLAGS)

clean:
	rm -f $(TARGETS)

.PHONY: all clean


