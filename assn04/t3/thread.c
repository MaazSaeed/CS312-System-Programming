#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>

#define BUF 10

char shared[BUF];
sem_t sem_empty;
sem_t sem_full;

void* producer(void* arg);
void* consumer(void* arg);
void check_error(int ret_val, char* function);

int elements = 0;

int main(void) {

	int ret_val;
	
	ret_val = sem_init(&sem_empty, 0, BUF); // start with 10 empty slots.
	check_error(ret_val, "sem_init");
	
	ret_val = sem_init(&sem_full, 0, 0);
	check_error(ret_val, "sem_init");
	
	pthread_t producer_thread;
	pthread_t consumer_thread;
	
	memset(shared, 0, sizeof(shared));
	
	ret_val = pthread_create(&producer_thread, NULL, producer, NULL);
	check_error(ret_val, "pthread_create -- producer");
	
	ret_val = pthread_create(&consumer_thread, NULL, consumer, NULL);
	check_error(ret_val, "pthread_create -- consumer");
	
	ret_val = pthread_join(producer_thread, NULL);
	check_error(ret_val, "pthread_join -- producer");
		
	ret_val = sem_destroy(&sem_full);
	check_error(ret_val, "sem_destroy -- sem_full");
	
	ret_val = pthread_join(consumer_thread, NULL);
	check_error(ret_val, "pthread_join -- consumer");
	
	ret_val = sem_destroy(&sem_empty);
	check_error(ret_val, "sem_destroy -- sem_empty");

	
	exit(EXIT_SUCCESS);
}

void* producer(void* arg) {
	int idx = 0;
	
	while(elements < BUF) {
		sem_wait(&sem_empty);
		fprintf(stdout, "Producer at work... -->");
		
		char next = 'A' + idx;
		shared[idx] = next;
		
		fprintf(stdout, " produced %c\n", next);
	
		idx = idx + 1;
		
		++elements;
		
		sem_post(&sem_full);
	}
	return NULL;
}

void *consumer(void* arg) {
	int idx = 0;

	while(idx < BUF) {
		sem_wait(&sem_full);
		fprintf(stdout, "Consumer consuming... --> consumed %c\n", shared[idx]);
		++idx;
		sem_post(&sem_empty);
	}
	return NULL;
}

void check_error(int ret_val, char* function) {
	if(-1 == ret_val) {
		fprintf(stderr, "%s failed\n", function);
		exit(EXIT_FAILURE);
	}
	return;
}



