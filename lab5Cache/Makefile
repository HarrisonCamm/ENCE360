LIBS = -lpthread
CC = gcc -Iinclude -I./src
CFLAGS = -O3  -funroll-loops -Wall 

.PHONY: default all clean

default: test_mul benchmark_mul benchmark_block
all: default


MATRIX_OBJ = matrix_mul.o matrix.o
TEST_OBJ = test_mul.o
BENCHMARK_BLOCK = benchmark_block.o
BENCHMARK_MUL = benchmark_mul.o


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

test_mul: $(MATRIX_OBJ) $(TEST_OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

benchmark_mul: $(MATRIX_OBJ) $(BENCHMARK_MUL)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

benchmark_block: $(MATRIX_OBJ) $(BENCHMARK_BLOCK)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)



clean:
	-rm -f *.o
	-rm -f benchmark_mul test_mul benchmark_block