CC=			gcc
MPICC=			mpicc
CFLAGS=			-g -Wall -Wno-unused-function -O2
WRAP_MALLOC=		-DUSE_MALLOC_WRAPPERS
DFLAGS=			-DHAVE_PTHREAD $(WRAP_MALLOC)

PROG=			MM-Suite-MPI

SUBDIRS= operations utils solvers ../lib/CBLAS ../utils ../operations

SUBCLEAN = $(addsuffix .clean,$(SUBDIRS))

OBJECTS= utils/utils-MPI.o ../utils/mmio.o ../utils/utils.o \
	operations/DMxV-MPI.o ../operations/VectorOperations.o \
	solvers/ConjugateGradient-MPI.o solvers/ConjugateGradientSolver-MPI.o

CBLAS_DIR = ../lib/CBLAS/lib/

LIBS = -L$(CBLAS_DIR) -lcblas -lm -lblas
INCLUDES =

.PHONY: all

.SUFFIXES:.c .o .cc

.c.o:
	$(MPICC) -c $(CFLAGS) $(DFLAGS) $(INCLUDES) $< -o $@

all:$(PROG)
	

.PHONY: subdirs $(SUBDIRS)
.PHONY: clean $(SUBCLEAN)

subdirs: $(SUBDIRS)


$(SUBDIRS):
	$(MAKE) -C $@
	
MM-Suite-MPI: MM-Suite-MPI.o subdirs
	$(MPICC) $(CFLAGS) $(DFLAGS) $(AOBJS) MM-Suite-MPI.o $(OBJECTS) -o $@  $(LIBS)

clean: $(SUBCLEAN)
	rm $(PROG) *.o

$(SUBCLEAN): %.clean:
	$(MAKE) -C $* clean
