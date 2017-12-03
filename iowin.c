#include "iowin.h"
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "err.h"
#include <ctype.h>
#define BSIZE (COLS)

/* future me read this!
 * https://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c
*/ 

int ioPage(int fd)
{
	WINDOW *outputWin;  // output to file
	WINDOW *inputWin; // input from file
	pthread_t ithr; // separate thread for input from fd
	int err;
	void *arg;

	if ((outputWin = newwin(3, COLS, 3, 0)) == NULL) {
		addErrMsg("error creating outputWin");
		return -1;
	}
	if ((inputWin = newwin(LINES - 3 - 3, COLS, 3 + 3, 0)) == NULL) {
		addErrMsg("error creating inputWin");
		return -1;
	}

	keypad(outputWin, TRUE);
	wborder(outputWin, ' ', ' ', ACS_HLINE, ACS_HLINE, ACS_HLINE, ACS_HLINE, ACS_HLINE, ACS_HLINE);
	wrefresh(outputWin);

	arg = malloc(sizeof(int) + sizeof(WINDOW*));
	*(int*)arg = fd;
	*(WINDOW**)(arg + sizeof(int)) = inputWin;
	scrollok(inputWin, TRUE);

	err = pthread_create(&ithr, NULL, readFileToOutput, arg);

	if (err != 0) {
		addErrMsg("problem creating thread");
		return -2;
	}

	readInputToFile(outputWin, fd);
	pthread_cancel(ithr);
	delwin(inputWin);
	delwin(outputWin);
	free(arg);
	refresh();

	return 0;
}

int getLine(WINDOW *win, char *buf, int bsize, int *length)
{
	int ch;
	int i;

	for (i = 0; i < bsize && (ch = wgetch(win)) != '\n'; ++i, ++buf) {
		switch(ch) { 
			case KEY_BACKSPACE: case KEY_DL:
				deleteChar(win);
				buf -= (i>0) + 1;
				i -= (i>0) + 1;
				break;
			case KEY_F(2):
				return KEY_F(2);
				break;
			default:
				waddch(win, ch);
				*buf = ch;
				break; 
		}
	}

	*buf = '\n';
	++i;

	
	wmove(win, 1, 0);
	wclrtoeol(win);

	*length = i;

	return 0;
}

void deleteChar(WINDOW *win)
{
	int y, x;
	getyx(win, y, x);

	if (x == 0)
		return;

	wmove(win, y, x-1);
	waddch(win, ' ');
	wmove(win, y, x-1);
}

void *readFileToOutput(void *fdAndWin)
{
	int fd = *(int*)fdAndWin;
	WINDOW *win = *(WINDOW**)(fdAndWin + sizeof(int));
	int n;

	char buf[BSIZE];

	while ((n = read(fd, buf, BSIZE)) != -1) {
		char *p = inputParseLine(buf, n);
		wprintw(win, "%s", p);
		free(p);
		wrefresh(win);
	}

	return 0;
}

int readInputToFile(WINDOW *win, int fd)
{
	int sc;
	int length = 0;

	char obuf[BSIZE];

	wmove(win, 1, 0);

	while ((sc = getLine(win, obuf, BSIZE - 1, &length)) == 0) {
		char *p = outputParseLine(obuf, &length);
		write(fd, p, length);
		free(p);
	} 

	return sc;
}


char *inputParseLine(char *line, int size)
{
	char *newLine = malloc(size * 4 * sizeof(char));

	char *lend = newLine;

	for (int i = 0; i < size; ++i) {
		if (line[i] <= 31 && line[i] >= 0 && line[i] != '\n' && line[i] != '\t') {
			char valstr[8];
			*lend++ = '\\';
			sprintf(valstr, "%d", line[i]);
			strcpy(lend, valstr);
			lend += strlen(valstr);
		} else if (line[i] == '\\') {
			*lend++ = '\\';
			*lend++ = '\\';
		} else {
			*lend++ = line[i];
		}
	}
	*lend = '\0';

	return newLine;
}

static int readSeq(char **s) 
{
	if (**s != '\\')
		return -2;
	(*s)++;
	if (**s == '\\') {
		(*s)++;
		return '\\';
	}

	int val = 0;

	for (int i = 0; i < 2; ++i) {
		if (!isdigit(**s))
			break;
		val = val * 10 + **s - 48;
		(*s)++;
	}

	return val;
}

char *outputParseLine(char *line, int *len)
{
	char *newLine = malloc(strlen(line) + 1);
	char *p = newLine;

	while (*line != '\n') {
		if (*line == '\\' && line[1] == '\n')
			break;
		if (*line == '\\') {
			*p++ = readSeq(&line);
		} else
			*p++ = *line++;
	}

	if (*line == '\n')
		*p++ = '\n';

	*len = p  - newLine;

	return newLine;
}












