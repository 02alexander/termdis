#ifndef _IOWIN_H
#define _IOWIN_H
#include <ncurses.h>

/* page for output to fd and input from it 
 * 
 */
int ioPage(int fd);


/* reads from fd and outputs it to window */
void *readFileToOutput(void *fdAndWin /* first 4 bytes is fd and the rest is
                                        pointer to WINDOW */ );

/* returns length */
int getLine(WINDOW *win, char *buf, int bufSize);

#endif