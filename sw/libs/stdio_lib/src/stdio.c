#include "stdio.h"

FILE *fopen(const char *filename, const char *mode){
  
  FILE * stream = 0;
  
  fqprintf(filename, 0, 0, 0, 0);
  
  fqprintf("\n", 0, 0, 0, 0);
  
  *(volatile int*)( FILE_CMD_BASE_ADDR ) = 0x0;
  
  return stream;
  
}

int fclose(FILE *stream){
  
  *(volatile int*)( FILE_CMD_BASE_ADDR ) = 0x1;
  
  return 1;
  
}

int fwrite(const void *ptr, int size, int nmemb, FILE *stream){
  
  int trans = size*nmemb;
  char *dest = (char*)ptr;
  int nb_trans = 0;
  
  while(nb_trans < trans){
    *(volatile char*) STREAM_BASE_ADDR = *dest;
    dest++;
    nb_trans++;
  }
  
  return nb_trans;
  
}

int fread(void *ptr, int size, int nmemb, FILE *stream){
  
  int trans = size*nmemb;
  char *dest = (char *)ptr;
  int nb_trans = 0;
  
  while(trans > 0){
    *dest = *(volatile char*) STREAM_BASE_ADDR;
    if (*dest == (char)EOF)
      break;
    dest++;
    nb_trans++;
  }
  
  return nb_trans;
  
}
