include Makefile.in
PROG=			MM-Suite

MPIDIR= MPI
SUBDIRS= io operations utils solvers #lib/CBLAS

SUBCLEAN = $(addsuffix .clean,$(SUBDIRS))

SOURCES  = $(wildcard io/*.c)
SOURCES += $(wildcard utils/*.c)
SOURCES += $(wildcard solvers/*.c)
SOURCES += $(wildcard operations/*.c)

OBJECTS  = $(patsubst %.c,%.o,$(SOURCES))

LIBS = -lopenblas -llapacke -lm
LIBS_ATLAS = -lcblas -llapacke -lm
INCLUDES =

.PHONY: all

.SUFFIXES: .c .o .cc

all: $(PROG)

.PHONY: subdirs $(SUBDIRS)
.PHONY: clean $(SUBCLEAN)

subdirs: $(SUBDIRS)


$(SUBDIRS):
	$(MAKE) -C $@

MPISuite:
	$(MAKE) -C $(MPIDIR)

cleanMPI:
	$(MAKE) -C $(MPIDIR) clean

MM-Suite: MM-Suite.o subdirs
	$(CC) $(CFLAGS) $(DFLAGS) $(AOBJS) MM-Suite.o $(OBJECTS) -o $@  $(LIBS)
	
atlas: MM-Suite.o subdirs
	$(CC) $(CFLAGS) $(DFLAGS) $(AOBJS) MM-Suite.o $(OBJECTS) -o $(PROG)  $(LIBS_ATLAS)

clean: $(SUBCLEAN)
	rm $(PROG) *.o

$(SUBCLEAN): %.clean:
	$(MAKE) -C $* clean
