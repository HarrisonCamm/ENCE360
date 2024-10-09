#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

#define NUM_THREADS 4 // Define number of threads

typedef double MathFunc_t(double);

double globalResult = 0; // Shared result variable
pthread_mutex_t resultMutex; // Mutex for controlling access to the global result

struct ThreadArgs {
    MathFunc_t *func;
    double rangeStart;
    double rangeEnd;
    size_t numSteps;
};

// Gaussian function
double gaussian(double x) {
    return exp(-(x * x) / 2) / (sqrt(2 * M_PI));
}

// Charge decay function
double chargeDecay(double x) {
    if (x < 0) {
        return 0;
    } else if (x < 1) {
        return 1 - exp(-5 * x);
    } else {
        return exp(-(x - 1));
    }
}

// Trapezoid integration method for a range
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
    globalResult += localArea;
    pthread_mutex_unlock(&resultMutex);

    return NULL;
}

// Function to validate input and select the appropriate mathematical function
bool getValidInput(MathFunc_t** func, char* funcName, double* start, double* end, size_t* numSteps) {
    memset(funcName, '\0', 10); // Clear funcName

    // Read input numbers and place them in the given addresses
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

    return (numRead == 4 && *func != NULL && *end >= *start && *numSteps > 0);
}

int main(void) {
    pthread_t threads[NUM_THREADS]; // Array of threads
    struct ThreadArgs threadArgs[NUM_THREADS]; // Array of arguments for threads

    double rangeStart;
    double rangeEnd;
    size_t numSteps;
    MathFunc_t* func;
    char funcName[10] = {'\0'};

    printf("Query format: [func] [start] [end] [numSteps]\n");

    while (getValidInput(&func, funcName, &rangeStart, &rangeEnd, &numSteps)) {
        globalResult = 0; // Reset global result

        double stepSize = (rangeEnd - rangeStart) / NUM_THREADS;
        size_t stepsPerThread = numSteps / NUM_THREADS;

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

        printf("The integral of function \"%s\" in range %g to %g is %.10g\n", funcName, rangeStart, rangeEnd, globalResult);
    }

    _exit(0); // Exit the program
}
