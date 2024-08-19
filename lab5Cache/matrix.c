#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>



//transpose a square matrix, first parameter is output
void matrix_transpose(double *res, double *a, size_t n) {
	
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < n; ++j) {
			res[i * n + j] = a[j * n + i];
		}
	}
	
}





//Fill a square matrix with zeroes
void zero_matrix(double *m, size_t n) {
	memset(m, 0, sizeof(double) * n * n);	
}

//Allocate a square matrix
double *alloc_matrix(size_t n) {
	double *res = (double*)malloc(sizeof(double) * n * n); 
	return res;
}


// Fill a square matrix with random values between 0 and 1
double *random_matrix(size_t n) {
	double *res = alloc_matrix(n);
	
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < n; ++j) {
			res[i * n + j] = drand48();
		}
	}
	
	return res;
}

// Compare two square matrices (a and b) for equality within a tolerance (eps)
// Returns: 1 equal, 0 not equal
int compare_matrix(double *a, double *b, size_t n) {
	
	double eps = 1e-6;

	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < n; ++j) {
			if (fabs(a[i * n + j] - b[i * n + j]) > eps) {
				return 0;
			}
		}
	}
	
	return 1;
}



double seconds(struct timespec start, struct timespec stop) {
	double diff = (stop.tv_sec - start.tv_sec) + (double)(stop.tv_nsec - start.tv_nsec) / (double)1e9;
	return diff;
}
