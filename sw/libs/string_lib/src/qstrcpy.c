#include "string_lib.h"

void qstrcpy (const char *s1, const char *s2)
{
  char *s1p = (char *)s1;
  char *s2p = (char *)s2;

  while (*s1p != '\0')
    {
      (*s2p) = (*s1p);

      ++s1p;
      ++s2p;
    }
}
