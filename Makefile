
CC = gcc

CFLAGS = -Wall -ftrapv -Wfloat-equal -Wstrict-prototypes -Wwrite-strings

TARGET = termdis

NCURSES = -lncurses

all:
	$(CC) $(CFLAGS) -pthread main.c pages/*.c -o $(TARGET) $(NCURSES)
