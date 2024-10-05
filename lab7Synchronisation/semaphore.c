#define _GNU_SOURCE


#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include <stdlib.h>

#include "assert.h"

#define NUM_THREADS 4

typedef struct {

	// Read and write semaphores for our channel 
	sem_t read;
	sem_t write;

	char *message; // Value stored in the channel

} Channel;


void channel_write(Channel *channel, void *message) {

	// _wait_ until the channel becomes empty, then write new contents 
	// signal any consumers waiting to read the channel
	//
	// You will need sem_post, sem_wait
	//
	// TODO: wait for our chance to access the variable

  //   assert(channel->message == NULL && "channel should be empty!");

	//write to the message  
	channel->message = message;

	// TODO: signal to any readers that there's an update


	// Channel is now occupied
}


void *channel_read(Channel *channel) {

	// TODO:
	// _wait_ until the channel becomes full, then read from the channel and take the contents 
	// signal any producers waiting to write to the channel
	//
	// You will need sem_post, sem_wait
	//
	void *message = NULL;
	// wait for an update from one of the threads

	// read the variable
	message = channel->message;
	channel->message = NULL;

	// signal to any threads waiting that they can send another update

	return message;

	// Channel is now empty
}


void channel_init(Channel *channel) {

	// TODO:
	// Initialize channel to an empty state ready for writing (and not reading)
	// You will need: sem_init
	//


	// TODO: Initialise count of the read semaphore to 0 (there's nothing to read yet)

	// TODO: Initialise the write semaphore is initialised to 1 (channel is empty, free for writing)

	channel->message = NULL;

}



// Producer thread sending messages to main
void *producer(void *arg)
{
	char message[128];
	Channel *channel = (Channel*)arg;
	for (int k = 0; k < 10; ++k) {
		sprintf(message, "Hello %d from thread %x", k, (unsigned int)pthread_self());
		// Send message to main
		channel_write(channel, message);
		sleep(1);
	}

	// Signal to the main thread that we have finished!
	channel_write(channel, NULL);
	pthread_exit(0);
}


int main(int argc, char **argv) {

	Channel channel;
	pthread_t thread[NUM_THREADS];

	// Initialise the Chan structure
	channel_init(&channel);

	// Create a bunch of threads running out producer
	for (int i = 0; i < NUM_THREADS; ++i) {
		pthread_create(&thread[i], NULL, producer, &channel);
	}

	int finished = 0;
	while (finished < NUM_THREADS) {
		// Consumer of the pi calculation
		char *message = channel_read(&channel);
		if (message) {
			printf("recieved: %s\n", message);
		}
		else {
			// Another worker has finished
			finished++;
		}
	}


	// Wait for all the threads we created
	for (int i = 0; i < NUM_THREADS; ++i) {
		// Wait for ith thread to finish
		pthread_join(thread[i], NULL);
	}


	return 0;
}


