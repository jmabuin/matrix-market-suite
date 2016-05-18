include ../Makefile.in

SOURCES = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SOURCES))

all: $(OBJS)

clean:
	rm -f $(OBJS)
