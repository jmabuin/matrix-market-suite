CC=			gcc
MPICC=			mpicc
CFLAGS=			-g -Wall -Wno-unused-function -O2 
WRAP_MALLOC=		-DUSE_MALLOC_WRAPPERS
DFLAGS=			-DHAVE_PTHREAD $(WRAP_MALLOC)

OBJS= utils-MPI.o

LIBS =
INCLUDES =

.c.o:
	$(MPICC) -c $(CFLAGS) $(DFLAGS) $(INCLUDES) $< -o $@

all: $(OBJS)

clean:
	rm *.o
