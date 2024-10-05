CC = gcc
CFLAGS =  -Wall -Werror -g -lrt -lm -lpthread

.PHONY: all clean

all: mutex semaphore



%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

fork: fork.c $(TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

mutex: mutex.c $(TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

semaphore: semaphore.c $(TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


clean:
	-rm -f *.o 
	-rm -f mutex semaphore
