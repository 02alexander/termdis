#include <ncurses.h>
#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "iowin.h"
#include "err.h"

int openPort(char *name, speed_t sp);

speed_t getSpeedFromStr(char *name);

int main(int argc, char *argv[])
{
	char *fname = NULL;
	speed_t brate = 0;

	if (argc <= 1) {
		printf("usage termdis file_name BAUDRATE\n");
		exit(1);
	} 

	for (int i = 1; i < argc; ++i) {
		if (isdigit(argv[i][0])) {
			brate = getSpeedFromStr(argv[i]);
			if (brate == -1) {
				printf("%s is not a valid BAUDRATE\n", argv[i]);
				exit(3);
			}
		} else
			fname = argv[i];
	}

	
	int fd = openPort(fname, brate);

	if (fd < 0) {
		endwin();
		if (fd == -1)
			fprintf(stderr, "couldn't open %s\n", fname);
		else if (fd == -2)
			fprintf(stderr, "%s is not a terminal device\n", fname);
		exit(2);
	}

	atexit( (void(*)(void))endwin);

	initscr();
	cbreak();
	keypad(stdscr, TRUE);	
	noecho();

	curs_set(0);

	ioPage(fd);

	endwin();
	printErrMsgs();

	exit(0);
}

int openPort(char *name, speed_t sp)
{
	int fd;
	struct termios term;

	if ((fd = open(name, O_RDWR | O_NOCTTY | O_SYNC)) == -1)
		return -1;
	if (!isatty(fd))
		return -2;

	tcgetattr(fd, &term);

	cfsetispeed(&term, sp);
	cfsetospeed(&term, sp);

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

speed_t getSpeedFromStr(char *name)
{
	if (strcmp(name, "50") == 0)
		return B50;
	if (strcmp(name, "75") == 0)
		return B75;
	if (strcmp(name, "110") == 0)
		return B110;
	if (strcmp(name, "134") == 0)
		return B134;
	if (strcmp(name, "150") == 0)
		return B150;
	if (strcmp(name, "200") == 0)
		return B200;
	if (strcmp(name, "300") == 0)
		return B300;
	if (strcmp(name, "600") == 0)
		return B600;
	if (strcmp(name, "1200") == 0)
		return B1200;
	if (strcmp(name, "1800") == 0)
		return B1800;
	if (strcmp(name, "2400") == 0)
		return B2400;
	if (strcmp(name, "4800") == 0)
		return B4800;
	if (strcmp(name, "9600") == 0)
		return B9600;
	if (strcmp(name, "19200") == 0)
		return B19200;
	if (strcmp(name, "38400") == 0)
		return B38400;
	if (strcmp(name, "57600") == 0)
		return B57600;
	if (strcmp(name, "115200") == 0)
		return B115200;
	return -1;
}
