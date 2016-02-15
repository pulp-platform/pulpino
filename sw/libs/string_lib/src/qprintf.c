// Copyright 2016 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "string_lib.h"
#include "utils.h"
#include "uart.h"
#include <stdarg.h>

#define PAD_RIGHT 1
#define PAD_ZERO  2

/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 32


static unsigned divu10(unsigned n) {
  unsigned q, r;

  q = (n >> 1) + (n >> 2);
  q = q + (q >> 4);
  q = q + (q >> 8);
  q = q + (q >> 16);
  q = q >> 3;
  r = n - q * 10;

  return q + ((r + 6) >> 4);
}

char remu10_table[16] = {
  0, 1, 2, 2, 3, 3, 4, 5,
  5, 6, 7, 7, 8, 8, 9, 0
};

static unsigned remu10(unsigned n) {
  n = (0x19999999 * n + (n >> 1) + (n >> 3)) >> 28;
  return remu10_table[n];
}



int putchar(int s)
{
  uart_sendchar(s);

  return s;
}

static void qprintchar(char **str, int c)
{
  if (str)
  {
    **str = c;
    ++(*str);
  }
  else
    putchar((char)c);
}

static int qprints(char **out, const char *string, int width, int pad)
{
  register int pc = 0, padchar = ' ';

  if (width > 0) {
    register int len = 0;
    register const char *ptr;
    for (ptr = string; *ptr; ++ptr) ++len;
    if (len >= width) width = 0;
    else width -= len;
    if (pad & PAD_ZERO) padchar = '0';
  }
  if (!(pad & PAD_RIGHT)) {
    for ( ; width > 0; --width) {
      qprintchar (out, padchar);
      ++pc;
    }
  }
  for ( ; *string ; ++string) {
    qprintchar (out, *string);
    ++pc;
  }
  for ( ; width > 0; --width) {
    qprintchar (out, padchar);
    ++pc;
  }

  return pc;
}

static int qprinti(char **out, int i, int b, int sg, int width, int pad, char letbase)
{
  char print_buf[PRINT_BUF_LEN];
  register char *s;
  register int neg = 0, pc = 0;
  unsigned int t,u = i;

  if (i == 0)
  {
    print_buf[0] = '0';
    print_buf[1] = '\0';
    return qprints (out, print_buf, width, pad);
  }

  if (sg && b == 10 && i < 0)
  {
    neg = 1;
    u = -i;
  }

  s = print_buf + PRINT_BUF_LEN-1;
  *s = '\0';

  // treat HEX and decimal differently
  if(b == 16) {
    // HEX
    while (u) {
      int t = u & 0xF;

      if (t >= 10)
        t += letbase - '0' - 10;

      *--s = t + '0';
      u >>= 4;
    }
  } else {
    // decimal
    while (u) {
      *--s = remu10(u) + '0';
      u = divu10(u);
    }
  }

  if (neg) {
    if( width && (pad & PAD_ZERO) )
    {
      qprintchar (out, '-');
      ++pc;
      --width;
    }
    else
    {
      *--s = '-';
    }
  }
  return pc + qprints (out, s, width, pad);
}

static int qprint(char **out, const char *format, va_list va)
{
  register int width, pad;
  register int pc = 0;
  char scr[2];

  for (; *format != 0; ++format)
  {
    if (*format == '%')
    {
      ++format;
      width = pad = 0;
      if (*format == '\0') break;
      if (*format == '%') goto out;
      if (*format == '-')
      {
        ++format;
        pad = PAD_RIGHT;
      }
      while (*format == '0')
      {
        ++format;
        pad |= PAD_ZERO;
      }
      for ( ; *format >= '0' && *format <= '9'; ++format) {
        width *= 10;
        width += *format - '0';
      }
      if( *format == 's' ) {
        register char *s = va_arg(va, char*);
        pc += qprints (out, s?s:"(null)", width, pad);
        continue;
      }
      if( *format == 'd' ) {
        pc += qprinti (out, va_arg(va, int), 10, 1, width, pad, 'a');
        continue;
      }
      if( *format == 'u' ) {
        pc += qprinti (out, va_arg(va, unsigned int), 10, 0, width, pad, 'a');
        continue;
      }
      if( *format == 'x' ) {
        pc += qprinti (out, va_arg(va, uint32_t), 16, 0, width, pad, 'a');
        continue;
      }
      if( *format == 'X' ) {
        pc += qprinti (out, va_arg(va, uint32_t), 16, 0, width, pad, 'A');
        continue;
      }
      if( *format == 'c' ) {
        scr[0] = va_arg(va, int);
        scr[1] = '\0';
        pc += qprints (out, scr, width, pad);
        continue;
      }
    }
    else
    {
out:
      qprintchar (out, *format);
      ++pc;
    }
  }
  if (out) **out = '\0';

  return pc;
}

int printf(const char *format, ...)
{
  int pc;
  va_list va;

  va_start(va, format);

  pc = qprint(0, format, va);

  va_end(va);

  return pc;
}


int puts(const char *s)
{
  int i=0;

  while(s[i] != '\0')
    putchar(s[i++]);

  putchar('\n');

  return i;
}

int strcmp (const char *s1, const char *s2)
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

char* strcpy (char *s1, const char *s2)
{
  char *s1p = (char *)s1;
  char *s2p = (char *)s2;

  while (*s2p != '\0')
  {
    (*s1p) = (*s2p);

    ++s1p;
    ++s2p;
  }

  return s1;
}

size_t strlen (const char *str)
{
  char *s = (char *)str;
  size_t len = 0;

  if (s == NULL)
    return 0;

  while (*s++ != '\0')
    ++len;
  return len;
}
