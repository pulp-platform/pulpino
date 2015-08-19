#include "stdio_lib.h"
#include "pulp.h"

FILE *fopen(const char *filename, const char *mode) {

  FILE * stream = 0;

  fprintf(0, filename);

  fprintf(0, "\n");

  *(volatile int*)( FILE_CMD_BASE_ADDR ) = 0x0;

  return stream;
}

int fclose(FILE *stream) {

  *(volatile int*)( FILE_CMD_BASE_ADDR ) = 0x1;

  return 1;
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {

  size_t trans = size*nmemb;
  char *dest = (char*)ptr;
  size_t nb_trans = 0;

  while(nb_trans < trans){
    *(volatile char*) STREAM_BASE_ADDR = *dest;
    dest++;
    nb_trans++;
  }

  return nb_trans;
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) {

  int trans = size*nmemb;
  char *dest = (char *)ptr;
  size_t nb_trans = 0;

  while(trans > 0){
    *dest = *(volatile char*) STREAM_BASE_ADDR;
    if (*dest == (char)EOF)
      break;
    dest++;
    nb_trans++;
  }

  return nb_trans;
}
