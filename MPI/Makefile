include Makefile.in
PROG=			MM-Suite-MPI

SUBDIRS= operations utils solvers ../utils ../operations

SUBCLEAN = $(addsuffix .clean,$(SUBDIRS))

SOURCES += $(wildcard utils/*.c)
SOURCES += $(wildcard ../utils/*.c)
SOURCES += $(wildcard solvers/*.c)
SOURCES += $(wildcard operations/*.c)
SOURCES += $(wildcard ../operations/*.c)

OBJECTS= $(patsubst %.c,%.o,$(SOURCES))

LIBS = -lopenblas -lm -llapacke
INCLUDES =

.PHONY: all

.SUFFIXES:.c .o .cc

.c.o:
	$(CC) -c $(CFLAGS) $(DFLAGS) $(INCLUDES) $< -o $@

all:$(PROG)
	

.PHONY: subdirs $(SUBDIRS)
.PHONY: clean $(SUBCLEAN)

subdirs: $(SUBDIRS)


$(SUBDIRS):
	$(MAKE) -C $@
	
MM-Suite-MPI: MM-Suite-MPI.o subdirs
	$(CC) $(CFLAGS) $(DFLAGS) $(AOBJS) MM-Suite-MPI.o $(OBJECTS) -o $@  $(LIBS)

clean: $(SUBCLEAN)
	rm $(PROG) *.o

$(SUBCLEAN): %.clean:
	$(MAKE) -C $* clean
