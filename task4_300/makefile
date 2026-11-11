# makefile from lecture 4

INCLDIR = ./include
CC = gcc
CFLAGS = -O2
CFLAGS += -I$(INCLDIR)

OBJDIR = obj

_DEPS = task4_prime.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = task4_prime.o task4_threads.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

task4: $(OBJS)
	gcc -o $@ $^ $(CFLAGS) -pthread

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o task4  *~ core $(INCLDIR)/*~

