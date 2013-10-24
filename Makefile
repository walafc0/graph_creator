CC      = gcc
CFLAGS  = -Wall -Werror -pedantic -std=c99
LDFLAGS = #
OBJS    = graph.o compute.o main.o
TRGT    = main

all: $(TRGT)
$(TRGT): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

clean:
	rm -f $(TRGT)
	rm -f $(OBJS)
