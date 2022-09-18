CC = cc
CFLAGS = -pthread
OBJECTS = main.o

all: main

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

main: $(OBJECTS)
	$(CC) main.o -o main

clean:
	rm -f *.o main
