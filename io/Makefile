CC=			gcc
CFLAGS=			-g -Wall -Wno-unused-function -O2 
WRAP_MALLOC=		-DUSE_MALLOC_WRAPPERS
DFLAGS=			-DHAVE_PTHREAD $(WRAP_MALLOC)

OBJS= CreateDenseMatrixSymmetric.o CreateDenseVector.o CreateDenseMatrixSymmetricRowLine.o \
	CreateDenseMatrixGeneral.o CreateDenseMatrixGeneralRowLine.o

LIBS =
INCLUDES =

.c.o:
	$(CC) -c $(CFLAGS) $(DFLAGS) $(INCLUDES) $< -o $@

all: $(OBJS)

clean:
	rm *.o
