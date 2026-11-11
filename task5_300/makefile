# makefile from lecture 4

INCLDIR = ./include
CC = gcc

-std=c89
CFLAGS = -O2
CFLAGS += -I$(INCLDIR)

OBJDIR = obj

_DEPS = ewpdef.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = task5.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

task5: $(OBJS)
	gcc -o $@ $^ $(CFLAGS) -lm

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o task5  *~ core $(INCLDIR)/*~
