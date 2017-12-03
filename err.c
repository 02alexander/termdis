#include "err.h"
#include <stdlib.h>
#include <stdio.h>

static const char **msgbuf = NULL; /* buffer that contains all the messages */
int nMsgs = 0;			     /* count of messages */
int maxMsg = 8; 			 

const char **getErrMsgs(void)
{
	return msgbuf;
}

void printErrMsgs(void)
{
	if (msgbuf == NULL) 
		return;

	for (int i = 0; i < nMsgs; ++i)
		fprintf(stderr, "%s\n", msgbuf[i]);
}

void addErrMsg(const char *msg)
{
	if (msgbuf == NULL) {
		msgbuf = malloc(maxMsg * sizeof(char*));
	} else if (nMsgs >= maxMsg) {
		maxMsg *= 2;
		msgbuf = realloc(msgbuf, maxMsg * sizeof(char*));
	}

	msgbuf[nMsgs++] = msg;
}