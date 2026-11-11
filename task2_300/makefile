# makefile from lecture 4

INCLDIR = ./include
CC = gcc
CFLAGS = -O2
CFLAGS += -I$(INCLDIR)

OBJDIR = obj

_DEPS = task2_numbers_metadata.h number_utilities.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = task2.o task2_fib.o task2_prim.o task2_kvad.o task2_cube.o task2_perf.o task2_abun.o task2_def.o task2_odd.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

task2: $(OBJS)
	gcc -o $@ $^ $(CFLAGS) -lm

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o task2 pgexam26_output.bin  *~ core $(INCLDIR)/*~
