/*
 * numSort.c - A mostly empty file. Write a program that sorts an array of numbers (the type doesn't really matter) using the library function qsort(). 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Comparison function for qsort
int compare(const void *a, const void *b) {
    int32_t int_a = *(int32_t*)a;
    int32_t int_b = *(int32_t*)b;

    if (int_a < int_b) return -1;
    if (int_a > int_b) return 1;
    return 0;
}

int main() {
    // Define and initialize an array of int32_t
    int32_t array[] = {9, 3, 1, 4, 7, 8, 2, 6, 5, 0};
    size_t array_size = sizeof(array) / sizeof(array[0]);

    // Print the original array
    printf("Original array:\n");
    for (size_t i = 0; i < array_size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    // Sort the array using qsort
    qsort(array, array_size, sizeof(int32_t), compare);

    // Print the sorted array
    printf("Sorted array:\n");
    for (size_t i = 0; i < array_size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    return 0;
}