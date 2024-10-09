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

#define MAX_CHILDREN 4 //Define maximum number of children allowed
#define NUM_THREADS 4 // Define number of threads

typedef double MathFunc_t(double);
static sem_t numFreeChildren;

double globalResult = 0; // Shared result variable
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
double integrateTrap(MathFunc_t* func, double rangeStart, double rangeEnd, size_t numSteps)
{
	double rangeSize = rangeEnd - rangeStart;
	double dx = rangeSize / numSteps;

	double area = 0;
	for (size_t i = 0; i < numSteps; i++) {
		double smallx = rangeStart + i*dx;
		double bigx = rangeStart + (i+1)*dx;

		area += dx * ( func(smallx) + func(bigx) ) / 2; // Would be more efficient to multiply area by dx once at the end. 
	}

	return area;
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
    pthread_t threads[NUM_THREADS]; // Array of threads
    struct ThreadArgs threadArgs[NUM_THREADS]; // Array of arguments for threads

	double rangeStart;
	double rangeEnd;
	size_t numSteps;
	MathFunc_t* func;
    pid_t childPid;
	char funcName[10] = {'\0'};
	
	sem_init(&numFreeChildren, 0, MAX_CHILDREN);
	signal(SIGCHLD, waitChild); // The child will never receive a SIGCHLD, so this is safe. 

	printf("Query format: [func] [start] [end] [numSteps]\n");

	while (true) {

        globalResult = 0; // Reset global result
        double stepSize = (rangeEnd - rangeStart) / NUM_THREADS;
        size_t stepsPerThread = numSteps / NUM_THREADS;

        pthread_mutex_init(&resultMutex, NULL); // Initialize the mutex

		sem_wait(&numFreeChildren); // Wait until there's a free child process slot

		if(getValidInput(&func, funcName, &rangeStart, &rangeEnd, &numSteps)) {
			childPid = fork();
			if(childPid < 0) { /* Error Handling */
				perror("Failed to fork");
				sem_post(&numFreeChildren);
			} else if (childPid == 0) { //child process
				double area = integrateTrap(func, rangeStart, rangeEnd, numSteps);
				printf("The integral of function \"%s\" in range %g to %g is %.10g\n", funcName, rangeStart, rangeEnd, area);
				_exit(0); // Force immediate exit in the child process
			}
		} else {
			printf("Invalid input, exiting.\n");
            break;
		}
	}
	
	sem_destroy(&numFreeChildren);
	_exit(0); // Forces more immediate exit than normal - **Use this to exit processes throughout the assignment!**
}