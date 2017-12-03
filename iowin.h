#ifndef _IOWIN_H
#define _IOWIN_H
#include <ncurses.h>


/*
    functions handling the panel for input and output
   
    what the panel should look like

	baudrate, filename

	---------------
	user write here to output
    ---------------

	input from file here



	the input will turn 0-32 to \0-32 and \ to \\

	the output will do a similar thing turn \0-32 to 0-32 adn \\ to \

	also if it encounters \ on the end of the line it won't send the newline
*/



/* page for output to fd and input from it */
int ioPage(int fd);


/* reads from fd and outputs it to window */
void *readFileToOutput(void *fdAndWin /* first 4 bytes is fd and the rest is
                                        pointer to WINDOW */ );

/* read to file  */
int readInputToFile(WINDOW *win, int fd);


/* 
 * parses the line for input
 * inputParseLine doesn't stop reading when it hits '\0', it stops only after size 
 * outputParseLine reads untill newline
 * returns the parsed line dynamicly allocated
*/
char *inputParseLine(char *line, int size);

char *outputParseLine(char *line, int *len);


/* returns 0 if no special characters has been typed else special character */
int getLine(WINDOW *win, char *buf, int bufSize, int *length);


/* replaces previous char with space unless it is on the beginning of the line */
void deleteChar(WINDOW *win);

#endif