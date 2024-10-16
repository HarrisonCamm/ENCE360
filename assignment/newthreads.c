#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Number of threads
#define NUM_THREADS 4

typedef double MathFunc_t(double);

// Function prototypes
double gaussian(double x);
double chargeDecay(double x);
bool getValidInput(MathFunc_t** func, char* funcName, double* start, double* end, size_t* numSteps);

// Struct to hold arguments for each thread
typedef struct {
    MathFunc_t* func;
    double rangeStart;
    double rangeEnd;
    size_t numSteps;
    double result;
} ThreadData;

// Mutex for shared variable
pthread_mutex_t mutex;
double globalArea = 0.0;

// Gaussian function
double gaussian(double x)
{
    return exp(-(x * x) / 2) / (sqrt(2 * M_PI));
}

// Charge decay function
double chargeDecay(double x)
{
    if (x < 0) {
        return 0;
    } else if (x < 1) {
        return 1 - exp(-5 * x);
    } else {
        return exp(-(x - 1));
    }
}

// Integrate using the trapezoid method with struct as argument
void* integrateTrap(void* args)
{
    ThreadData* data = (ThreadData*) args;
    double dx = (data->rangeEnd - data->rangeStart) / data->numSteps;
    double area = 0;

    for (size_t i = 0; i < data->numSteps; i++) {
        double x1 = data->rangeStart + i * dx;
        double x2 = data->rangeStart + (i + 1) * dx;
        area += dx * (data->func(x1) + data->func(x2)) / 2;
    }

    // Lock mutex before updating the global result
    pthread_mutex_lock(&mutex);
    globalArea += area;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

// Function to parse input and set the function pointer
bool getValidInput(MathFunc_t** func, char* funcName, double* start, double* end, size_t* numSteps)
{
    memset(funcName, '\0', 10); // Clear funcName

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

int main(void)
{
    double rangeStart, rangeEnd;
    size_t numSteps;
    MathFunc_t* func;
    char funcName[10] = {'\0'};
    pthread_t threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];

    printf("Query format: [func] [start] [end] [numSteps]\n");

    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);

    while (getValidInput(&func, funcName, &rangeStart, &rangeEnd, &numSteps)) {
        globalArea = 0; // Reset global area

        // Split the range and steps between threads
        double stepSize = (rangeEnd - rangeStart) / NUM_THREADS;
        size_t stepsPerThread = numSteps / NUM_THREADS;

        for (int i = 0; i < NUM_THREADS; i++) {
            threadData[i].func = func;
            threadData[i].rangeStart = rangeStart + i * stepSize;
            threadData[i].rangeEnd = rangeStart + (i + 1) * stepSize;
            threadData[i].numSteps = stepsPerThread;

            // Create thread
            pthread_create(&threads[i], NULL, integrateTrap, &threadData[i]);
        }

        // Join threads
        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }

        // Output the result
        printf("The integral of function \"%s\" in range %g to %g is %.10g\n", funcName, rangeStart, rangeEnd, globalArea);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    _exit(0); // Exit program
}
