#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <malloc.h>
#include <stdint.h>


typedef struct {
    char random;
    int8_t junk;
} Data_t;

Data_t* newData(char random, int8_t junk) {
    Data_t* data = malloc(sizeof(Data_t));
    data->random = random;
    data->junk = junk;
    return data; 
}

void freeData(Data_t* data) {
    free(data);
}


int16_t* ramp(size_t n)  {
    int16_t* array = calloc(sizeof(int16_t), n);
    for (size_t i = 0; i < n; i++)
    {
        array[i] = (int16_t)(i + 1);
    }
    return array;
}


int main(int argc, char** argv) {
    for (int i = 0; i < argc; i++)
    {
        printf("[%d]%s\n", i, argv[i]);
    }
}


int64_t (*mulfunc)(int32_t, uint32_t) = &multiply;