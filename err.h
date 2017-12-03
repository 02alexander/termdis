#ifndef _ERR_H
#define _ERR_H

void addErrMsg(const char *msg);

const char **getErrMsgs(void);

void printErrMsgs(void);

#endif