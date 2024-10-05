#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#define NUM_THREADS 16
#define N 10000


typedef struct {
	float *total;
	size_t n;

	pthread_mutex_t *lock;
	pthread_t thread;

} Worker;


void *runSummation(void *ptr)
{
	Worker *worker = (Worker*)ptr;

	for (size_t i = 0; i < worker->n; i++) {
		//TODO: make this thread safe!!

		(*worker->total)++;
	}

	return NULL;
}



int main(void)
{

	// Global variable for total summation so far
	float total = 0;

	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	Worker workers[NUM_THREADS];


	for (size_t i = 0; i < NUM_THREADS; i++) {
		// What would be the problem declaring Worker w here?

		Worker *worker = &workers[i];
		worker->total = &total; // Pass the global total into each thread

		worker->lock = &lock;
		worker->n = N;

		//TODO: Make this run in a thread!
		runSummation((void*)worker);
	}


	////////////////////////////////
	// Wait for all the threads we created
	// for (size_t i = ...)
	////////////////////////////////

	printf("Final total: %f \n", total);

	return 0;
}

