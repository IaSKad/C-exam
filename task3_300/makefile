INCLDIR = ./include
CC = gcc
CFLAGS = -O2
CFLAGS += -I$(INCLDIR)

OBJDIR = obj

_DEPS = task3.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = task3.o main.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

task3: $(OBJS)
	gcc -o $@ $^ $(CFLAGS) -lm

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o task3
