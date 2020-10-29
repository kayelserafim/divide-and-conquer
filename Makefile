LAD_ENV := $(shell command -v ladcomp 2> /dev/null)

S_DIR = ./src

CC = gcc
MPI = mpicc
LAD_CMD = ladcomp -env mpicc

CFLAGS = -Wall -g

all: sequential_bs.o sequential_qs.o parallel_bs.o

sequential_bs.o: $(S_DIR)/sequential_bs.c
	$(CC) -o $@ $< $(CFLAGS)
	
sequential_qs.o: $(S_DIR)/sequential_qs.c
	$(CC) -o $@ $< $(CFLAGS)
	
parallel_bs.o: $(S_DIR)/parallel_bs.c
ifdef LAD_ENV
	$(LAD_CMD) -o $@ $< $(CFLAGS)
else
	$(MPI) -o $@ $< $(CFLAGS)
endif

.PHONY: clean

clean:
	rm -rf *.o *~ all