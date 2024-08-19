
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>


#include "matrix.h"

// Naive matrix multiplication 
// Multiply two square matrices of size n
// return output in res pointer 
void matrix_mul_basic(double *res, double *a, double *b, size_t n) {
	
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < n; ++j) {
			
			double r = 0.0;

			// Compute res(i, j) as dot product of ith row of a and jth column of b
			// Sum entries
			for (size_t k = 0; k < n; ++k) {
				r += a[i * n + k] * b[k * n + j];
			}
			
			res[i * n + j] = r;
		}
	}
	
}

 
// Improve upon the above matrix multiplication by first transposing 
// the second matrix.
//
// Run the test provided ./test_mul	to test correctness
// then run the benchmark with ./benchmark_mul to compare the differences
//
// What impact does the transposition have on performance? Why?
// 

void matrix_mul_transposed(double *res, double *a, double *b, size_t n) {
	double *bt = alloc_matrix(n);
	matrix_transpose(bt, b, n);
	
	// TODO: implement matrix multiplication between a and transposed matrix bt

	free(bt);
	
}

size_t min(size_t a, size_t b) {
	return a > b ? b : a; 
}

//
// Improve upon the naive matrix multiplication using loop blocking
// the parameter for the inner loop size is passed as the last parameter
//
// As above test your implementation before benchmarking performance
// Can you beat matrix_mul_transposed? 
//
// What is the best block size? Why? Experiment with ./benchmark_block
//
// Note: helping gcc with array indexing by pre-computing common values helps a great deal to improve performance here
// 
//

void matrix_mul_blocked(double *res, double *a, double *b, size_t n, size_t block) {
	
	zero_matrix(res, n);
	
	// loop over outer blocks 
	for (size_t i = 0; i < n; i += block) {
		for (size_t j = 0; j < n; j += block) { 
			for (size_t k = 0; k < n; k += block) {
				
				int i_end = min(i + block, n);
				int j_end = min(j + block, n);
				int k_end = min(k + block, n);
				
				// loop over inner block (indices ii, jj, kk)
				
				//TODO: finish blocked matrix implementation
			}
		}
	}
	
	
}











