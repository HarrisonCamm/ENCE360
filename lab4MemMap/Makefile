LIBS = -lpthread
CC = gcc -Iinclude -std=gnu99
CFLAGS = -g -Wall -O2

.PHONY: default all clean

default: mmap realloc
all: default

DEPS = 
MMAP_OBJ = mmap.o 
REALLOC_OBJ = realloc.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mmap: $(MMAP_OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)


realloc: $(REALLOC_OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)
	

clean:
	-rm -f *.o
	-rm -f realloc mmap