#include "string_lib.h"

int qstrcmp (const char *s1, const char *s2)
 {
  /* No checks for NULL */
  char *s1p = (char *)s1;
  char *s2p = (char *)s2;

  while (*s2p != '\0')
    {
      if (*s1p != *s2p)
	break;

      ++s1p;
      ++s2p;
    }
  return (*s1p - *s2p);
}

