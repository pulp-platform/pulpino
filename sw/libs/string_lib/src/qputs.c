#include "string_lib.h"

void qputs(char *s)
{
  int i=0;
  while(s[i]!='\0') qputchar(s[i++]);      
}
