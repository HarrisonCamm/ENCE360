CC = mpicc.openmpi -Iinclude -I./src
# CFLAGS =  -Wall -lrt -lm -O3 -funroll-loops
CFLAGS =  -Wall -lrt -lm -g

.PHONY: default all clean

default: hello_world pass_the_parcel ping vector_len sort permissions
all: default



%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

hello_world: hello_world.c $(TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

pass_the_parcel: pass_the_parcel.c $(TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ping: ping.c $(TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

vector_len: vector_len.c $(TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)	
	
sort: sort.c $(TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

permissions:
	chmod +x ./run.sh && chmod +x run_remote.sh


clean:
	-rm -f hello_world pass_the_parcel ping vector_len sort
