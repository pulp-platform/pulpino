#include <stdio.h>
#include "utils.h"
#include "bar.h"

__attribute__ ((section(".heapsram"))) int in_buffer[16];
__attribute__ ((section(".heapsram"))) int out_buffer[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

int main()
{
  FILE * buffer;

  if (get_core_id() == 0){

    printf("Starting test for STDIO...\n");

    buffer = fopen("test_file.txt","rw");

    fread((void*)in_buffer, 16, 4, buffer );

    //printf((char*)in_buffer);



    fwrite((void *)"Buonasera File system", 1, 21, buffer);

    fclose(buffer);

    printf("Test for STDIO2 complete!!!\n");

  }

  synch_barrier();

  eoc(0);

  return 0;
}
