#ifndef _FLAGHANDLER_H
#define _FLAGHANDLER_H

/*
	this panel is for showing all the flags and changing them by pressing enter.
	currently marked flag will be colour highlighted.

	it will print the type of flag then all the option in columns. 
	it should try to print as many as possible per row.

	if there is too many flag too be diplayed on screen you will be able to scroll

	| c_iflag	  |
	| ... ... ... |
	| ... ... ... |
	| c_oflag	  |
	| ... ... ... |
	| ... ... ... |
*/

struct termios;

int flagSettingsPage(int fd, struct termios *terminfo)



#endif