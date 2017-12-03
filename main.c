#include <ncurses.h>
#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "iowin.h"
#include "err.h"

int openPort(const char *name);

int main(int argc, char const *argv[])
{
	atexit( (void(*)(void))endwin);

	const char *fname = "/dev/ttyACM0";

	initscr();
	cbreak();
	keypad(stdscr, TRUE);	
	noecho();

	curs_set(0);

	int fd = openPort(fname);

	if (fd < 0) {
		endwin();
		if (fd == -1)
			fprintf(stderr, "couldn't open %s\n", fname);
		else if (fd == -2)
			fprintf(stderr, "%s is not a terminal device\n", fname);
		return 1;
	}

	ioPage(fd);

	endwin();
	printErrMsgs();


	return 0;
}

int openPort(const char *name)
{
	int fd;
	struct termios term;

	if ((fd = open(name, O_RDWR | O_NOCTTY | O_SYNC)) == -1)
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

	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 2;

	tcsetattr(fd, TCSAFLUSH, &term);
	return fd;
}