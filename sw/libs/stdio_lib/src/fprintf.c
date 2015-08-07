
#include "string_lib.h"

void fputchar(char s)
{
  *(volatile int*)( FPUTCHAR_BASE_ADDR ) = s;
}

void fqprintchar(char **str, int c)
{
  if (str)
    {
      **str = c;
      ++(*str);
    }
  else 
    fputchar((char)c);
}

int fqprints(char **out, const char *string, int width, int pad)
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
      fqprintchar (out, padchar);
      ++pc;
    }
  }
  for ( ; *string ; ++string) {
    fqprintchar (out, *string);
    ++pc;
  }
  for ( ; width > 0; --width) {
    fqprintchar (out, padchar);
    ++pc;
  }

  return pc;
}

int fqprinti(char **out, int i, int b, int sg, int width, int pad, char letbase)
{
  char print_buf[PRINT_BUF_LEN];
  register char *s;
  register int neg = 0, pc = 0;
  unsigned int t,u = i;
	
  if (i == 0) 
    {
      print_buf[0] = '0';
      print_buf[1] = '\0';
      return fqprints (out, print_buf, width, pad);
    }

  if (sg && b == 10 && i < 0) 
    {
      neg = 1;
      u = -i;
    }

  s = print_buf + PRINT_BUF_LEN-1;
  *s = '\0';
        
  while (u) {
    t = u % b;
    if( t >= 10 )
      t += 'a' - '0' - 10;		  
    *--s = t + '0';
    u /= b;
  }

  if (neg) {
    if( width && (pad & PAD_ZERO) ) 
      {
	fqprintchar (out, '-');
	++pc;
	--width;
      }
    else 
      {
	*--s = '-';
      }
  }
  return pc + fqprints (out, s, width, pad);
}

int fqprint(char **out, int *varg)
{
  register int width, pad;
  register int pc = 0;
  register char *format = (char *)(*varg++);
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
	    register char *s = *((char **)varg++);
	    pc += fqprints (out, s?s:"(null)", width, pad);
	    continue;
	  }
	  if( *format == 'd' ) {
	    pc += fqprinti (out, *varg++, 10, 1, width, pad, 'a');
	    continue;
	  }
	  if( *format == 'x' ) {
	    pc += fqprinti (out, *varg++, 16, 0, width, pad, 'a');
	    continue;
	  }
	  if( *format == 'X' ) {
	    pc += fqprinti (out, *varg++, 16, 0, width, pad, 'A');
	    continue;
	  }
	  if( *format == 'u' ) {
	    pc += fqprinti (out, *varg++, 10, 0, width, pad, 'a');
	    continue;
	  }
	  if( *format == 'c' ) {
	    scr[0] = *varg++;
	    scr[1] = '\0';
	    pc += fqprints (out, scr, width, pad);
	    continue;
	  }
	}
      else 
	{
	out:
	  fqprintchar (out, *format);
	  ++pc;
	}
    }
  if (out) **out = '\0';
  return pc;
}

int fqprintf(const char *format, int arg1,int arg2,int arg3,int arg4)
{
  
  int __varg[5];
  
  __varg[0]= (int)(format);
  __varg[1]= (int)arg1;
  __varg[2]= (int)arg2;
  __varg[3]= (int)arg3;
  __varg[4]= (int)arg4;
  
  return fqprint(0, __varg);
  
}
