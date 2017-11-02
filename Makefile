CC = gcc

CFLAGS = -Wall -Wfloat-equal -Wpointer-arith -Wstrict-prototypes -Wwrite-strings
GTK = `pkg-config --libs --cflags gtk+-3.0`

all:
	$(CC) -o program *.c $(CFLAGS) $(GTK)
