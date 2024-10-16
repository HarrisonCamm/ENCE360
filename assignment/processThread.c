#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <semaphore.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <sys/wait.h>

#define MAX_CHILDREN 8 //Define maximum number of children allowed
#define NUM_THREADS 4 // Define number of threads

typedef double MathFunc_t(double);
static sem_t numFreeChildren;

double globalArea = 0; // Shared result variable
pthread_mutex_t resultMutex; // Mutex for controlling access to the global result

struct ThreadArgs {
    MathFunc_t *func;
    double rangeStart;
    double rangeEnd;
    size_t numSteps;
};

 /* Function prototypes */
void sigquit(int sigNum);
void waitChild(int sigNum);


double gaussian(double x)
{
	return exp(-(x*x)/2) / (sqrt(2 * M_PI));
}


double chargeDecay(double x)
{
	if (x < 0) {
		return 0;
	} else if (x < 1) {
		return 1 - exp(-5*x);
	} else {
		return exp(-(x-1));
	}
}





// Integrate using the trapezoid method.
void* integrateTrap(void *args) {
    struct ThreadArgs *threadArgs = (struct ThreadArgs *)args;
    MathFunc_t* func = threadArgs->func;
    double rangeStart = threadArgs->rangeStart;
    double rangeEnd = threadArgs->rangeEnd;
    size_t numSteps = threadArgs->numSteps;

    double rangeSize = rangeEnd - rangeStart;
    double dx = rangeSize / numSteps;
    double localArea = 0;

    for (size_t i = 0; i < numSteps; i++) {
        double smallx = rangeStart + i * dx;
        double bigx = rangeStart + (i + 1) * dx;
        localArea += dx * (func(smallx) + func(bigx)) / 2;
    }


    // Lock the mutex before updating the global result
    pthread_mutex_lock(&resultMutex);
    globalArea += localArea;
    pthread_mutex_unlock(&resultMutex);

    return NULL;
}




bool getValidInput(MathFunc_t** func, char* funcName, double* start, double* end, size_t* numSteps)
{
	memset(funcName, '\0', 10); // Clear funcName. Magic number used because format strings are annoying. 

	// Read input numbers and place them in the given addresses:
	size_t numRead = scanf("%9s %lf %lf %zu", funcName, start, end, numSteps);

	if (strcmp(funcName, "sin") == 0) {
		*func = &sin;
	} else if (strcmp(funcName, "gauss") == 0) {
		*func = &gaussian;
	} else if (strcmp(funcName, "decay") == 0) {
		*func = &chargeDecay;
	} else {
		*func = NULL;
	}

	// Return whether the given func and range is valid:
	return (numRead == 4 && *func != NULL && *end >= *start && *numSteps > 0);
}


void waitChild(int sigNum) {
    sem_post(&numFreeChildren);
}



int main(void)
{
    struct timespec start, end;
    sem_init(&numFreeChildren, 0, MAX_CHILDREN);
    signal(SIGCHLD, waitChild); // Signal handler for completed child processes

    double rangeStart;
    double rangeEnd;
    size_t numSteps;
    MathFunc_t* func;
    pid_t childPid;
    char funcName[10] = {'\0'};

    printf("Query format: [func] [start] [end] [numSteps]\n");

    // Start timing (parent process)
    clock_gettime(CLOCK_MONOTONIC, &start);

    while (true) {

        sem_wait(&numFreeChildren); // Wait until there's a free child process slot

        if (getValidInput(&func, funcName, &rangeStart, &rangeEnd, &numSteps)) {
            childPid = fork();
            if (childPid < 0) { // Error Handling
                perror("Failed to fork");
                sem_post(&numFreeChildren);
            } else if (childPid == 0) {
                // Child process starts here
                globalArea = 0; // Reset global area
                pthread_t threads[NUM_THREADS]; // Array of threads
                struct ThreadArgs threadArgs[NUM_THREADS]; // Array of arguments for threads

                double stepSize = (rangeEnd - rangeStart) / NUM_THREADS;
                size_t stepsPerThread = numSteps / NUM_THREADS;

                // Start timing for child process
                clock_gettime(CLOCK_MONOTONIC, &start);

                pthread_mutex_init(&resultMutex, NULL); // Initialize the mutex

                // Create threads to process the integration in parallel
                for (int i = 0; i < NUM_THREADS; i++) {
                    threadArgs[i].func = func;
                    threadArgs[i].rangeStart = rangeStart + i * stepSize;
                    threadArgs[i].rangeEnd = rangeStart + (i + 1) * stepSize;
                    threadArgs[i].numSteps = stepsPerThread;

                    pthread_create(&threads[i], NULL, integrateTrap, (void *)&threadArgs[i]);
                }

                // Wait for all threads to finish
                for (int i = 0; i < NUM_THREADS; i++) {
                    pthread_join(threads[i], NULL);
                }

                pthread_mutex_destroy(&resultMutex); // Destroy the mutex

                // End timing for child process
                clock_gettime(CLOCK_MONOTONIC, &end);

                // Calculate the time difference in seconds for the child process
                double timeTaken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

                printf("The integral of function \"%s\" in range %g to %g is %.10g\n", funcName, rangeStart, rangeEnd, globalArea);
                printf("Time taken: %.6f seconds\n", timeTaken);
                
                _exit(0); // Terminate child process
            }
        } else {
            printf("Invalid input, exiting.\n");
            break;
        }
    }

    // Wait for all child processes to complete before stopping the timer
    while (waitpid(-1, NULL, 0) > 0); // Wait for all children to terminate

    // End timing for the entire program (parent process)
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate the total time difference for the entire program
    double totalTimeTaken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Total time taken by parent and all children: %.6f seconds\n", totalTimeTaken);

    sem_destroy(&numFreeChildren); // Cleanup
    _exit(0); // Terminate parent process
}
