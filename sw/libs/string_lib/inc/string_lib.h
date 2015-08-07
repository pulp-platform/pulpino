#ifndef STRING
#define STRING

#include <stddef.h>

#include "pulp.h"

#define _stdout STDOUT_BASE_ADDR

#define PAD_RIGHT 1
#define PAD_ZERO  2

/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 32

int qstrlen (const char *str);
int  qstrcmp (const char *s1, const char *s2);
void qstrcpy (const char *s1, const char *s2);
void qputs(char *s);
int  qprintf(const char *format, int arg1,int arg2,int arg3,int arg4);
void qputchar(char s);
void _stdout_init(int local_id);

#endif
