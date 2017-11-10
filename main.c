#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define BSIZE 24

int openPort(const char *name);

int main(int argc, char const *argv[])
{
	initscr();
	cbreak();
	keypad(stdscr, TRUE);	
	noecho();



	getch();
	endwin();
	return 0;
}

int openPort(const char *name)
{
	int fd;
	struct termios term;

	if ((fd = open(name, O_RDWR)) == -1)
		return -1;
	if (!isatty(fd))
		return -2;

	tcgetattr(fd, &term);

	cfsetispeed(&term, B9600);
	cfsetospeed(&term, B9600);

	term.c_cflag |= CS8;

	term.c_oflag &= ~(ONLCR | ONOCR | OCRNL | OLCUC);
	term.c_oflag |= ONLRET;

	term.c_lflag &= ~(ECHO | ICANON);

	term.c_iflag &= ~(INPCK | ISTRIP);

	tcsetattr(fd, TCSAFLUSH, &term);
	return fd;
}