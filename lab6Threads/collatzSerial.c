#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NUM_TESTS 8

// This is a helper function for collatzSweep(). 
uint64_t collatzWalk(uint32_t initNum) {
	uint64_t numSteps = 0;
	uint64_t num = initNum;
	while (num != 1) { // This will always end, at least for initNum < UINT32_MAX. 
		numSteps++;
		if (num % 2 == 0) { // If it's even:
			num /= 2;
		} else { // If it's odd:
			num = 3*num + 1;
		}
	}

	return numSteps;
}


// Ignore me for later. 
void collatzSweep(uint32_t maxStart) {
	uint64_t totalSteps = 0;

	for (uint32_t i = 1; i <= maxStart; i++) {
		totalSteps += collatzWalk(i);
	}

	printf("Sweep to %u: %lu total steps\n", maxStart, totalSteps);
}


int main(void) {
	uint32_t testValues[NUM_TESTS] = {8400511, 1234567, 9876543, 5102234, 8246889, 3453451, 6666666, 6372812};
	
	// pthread_t threads[NUM_TESTS]; // Uncomment me when you start to parallelise. 

	for (size_t i = 0; i < NUM_TESTS; i++) {
		collatzSweep(testValues[i]);
	}
}