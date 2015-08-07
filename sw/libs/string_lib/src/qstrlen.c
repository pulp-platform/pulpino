#include "string_lib.h"

int qstrlen (const char *str)
{
        char *s = (char *)str;
        int len = 0;

        if (s == NULL)
                return 0;

        while (*s++ != '\0')
                ++len;
        return len;
}
