CC = cc
CFLAGS = -Wall
CURSES = -lncurses

all: siritori

siritori: main_1.o func_1.o
	$(CC) $(CFLAGS) -o siritori main_1.o func_1.o $(CURSES)

main_1.o: main_1.c
	$(CC) $(CFLAGS) -c main_1.c

func_1.o: func_1.c func_1.h
	$(CC) $(CFLAGS) -c func_1.c

clean:
	rm -f *.o siritori