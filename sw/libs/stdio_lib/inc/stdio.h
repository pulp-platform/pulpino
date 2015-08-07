#ifndef __STDIO_H__
#define __STDIO_H__

#include "pulp.h"

#define FILE int
#define EOF 0xFFFFFFFF

FILE *fopen(const char *filename, const char *mode);
int   fclose(FILE *stream);
int   fwrite(const void *ptr, int size, int nmemb, FILE *stream);
int   fread(void *ptr, int size, int nmemb, FILE *stream);

int fqprintf(const char *format, int arg1,int arg2,int arg3,int arg4);

#endif
