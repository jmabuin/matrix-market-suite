CC=			gcc
CFLAGS=			-g -Wall -Wno-unused-function -O2 
WRAP_MALLOC=		-DUSE_MALLOC_WRAPPERS
DFLAGS=			-DHAVE_PTHREAD $(WRAP_MALLOC)

OBJS= DMxV.o VectorOperations.o LUDecomposition.o \
	DMxDM.o

LIBS =
INCLUDES =

.c.o:
	$(CC) -c $(CFLAGS) $(DFLAGS) $(INCLUDES) $< -o $@

all: $(OBJS)

clean:
	rm *.o
