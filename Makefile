CC      = gcc
CFLAGS  = -Wall -Werror -pedantic -std=c99
LFLAGS  = #
DEFINES = #
OBJ     = graph.o compute.o main.o
NAME    = main

all: $(OBJ)
	$(CC) $(CFLAGS) $(DEFINES) $(LFLAGS) $(OBJ) -o $(NAME)

%o: %c
	$(CC) $(CFLAGS) $(DEFINES) $(LFLAGS) -c $^ -o $@

clean:
	rm -f $(NAME)
	rm -f *.o
