#include "iowin.h"
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#define BSIZE (COLS) /* bsize is width of terminal to make 
					 sure it wont try to print outside of terminal */

int ioPage(int fd)
{
	WINDOW *outputWin;
	WINDOW *inputWin;
	pthread_t ithr; // separate thread for input from fd
	int err;
	void *arg;

	inputWin = newwin(3, COLS, 4, 0);

	wborder(inputWin, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h');

	arg = malloc(sizeof(int) + sizeof(WINDOW*));
	*(int*)arg = fd;
	*(WINDOW**)(arg + sizeof(int)) = inputWin;
	err = pthread_create(&ithr, NULL, readFileToOutput, arg);

	if (err != 0) {
		printw("problem creating thread");
		return -1;
	}



	return 0;
}

void *readFileToOutput(void *fdAndWin)
{
	int fd = *(int*)fdAndWin;
	WINDOW *win = *(WINDOW**)(fdAndWin + sizeof(int));
	int n;

	char buf[BSIZE];

	while ((n = read(fd, buf, BSIZE)) != -1) {
		
	}

	return 0;
}