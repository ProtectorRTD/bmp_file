CFLAGS=-std=c11 -Wall -Werror
LDLIBS=-lm -lcurses
OUTPUT=make

all: $(OUTPUT)

$(OUTPUT): bmp.o transformations.o main.o
	gcc $(CFLAGS) bmp.o transformations.o main.o $(LDLIBS) -o $(OUTPUT)

main.o: main.c
	gcc $(CFLAGS) -c -g main.c $(LDLIBS) -o main.o

bmp.o: bmp.c bmp.h
	gcc $(CFLAGS) -c -g bmp.c $(LDLIBS) -o bmp.o

transformations.o: transformations.c transformations.h
	gcc $(CFLAGS) -c -g transformations.c $(LDLIBS) -o transformations.o


clean:
	rm -rf *.o
