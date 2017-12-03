
CC = gcc

CFLAGS = -Wall -ftrapv -Wfloat-equal -Wstrict-prototypes

TARGET = termdis

LIBS = -lncurses

all:
	$(CC) $(CFLAGS) -pthread *.c -o $(TARGET) $(LIBS)
