/* Copyright (C) 2017 ETH Zurich, University of Bologna
 * All rights reserved.
 *
 * This code is under development and not yet released to the public.
 * Until it is released, the code is under the copyright of ETH Zurich and
 * the University of Bologna, and may contain confidential and/or unpublished
 * work. Any reuse/redistribution is strictly forbidden without written
 * permission from ETH Zurich.
 *
 * Bug fixes and contributions will eventually be released under the
 * SolderPad open hardware license in the context of the PULP platform
 * (http://www.pulp-platform.org), under the copyright of ETH Zurich and the
 * University of Bologna.
 */

#include <byteswap.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/mman.h>

#define GPIO_AXI_ADDR   0x51030000


#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

int gpio_read() {
  char* gpio_map = MAP_FAILED;
  char* gpio_base;
  int mem_fd;
  int retval = 0;

  if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
    printf("can't open /dev/mem \n");

    retval = -1;
    goto fail;
  }

  gpio_map = (char*)mmap(
      NULL,
      MAP_SIZE,
      PROT_READ|PROT_WRITE,
      MAP_SHARED,
      mem_fd,
      GPIO_AXI_ADDR & ~MAP_MASK
      );


  if (gpio_map == MAP_FAILED) {
    perror("mmap error\n");

    retval = -1;
    goto fail;
  }

  gpio_base = gpio_map + (GPIO_AXI_ADDR & MAP_MASK);
  volatile uint32_t* gpio = (volatile uint32_t*)(gpio_base + 0x0);
  volatile uint32_t* dir  = (volatile uint32_t*)(gpio_base + 0x4);

  printf("GPIO has value %08X\n", *gpio);
  printf("DIR  has value %08X\n", *dir);


fail:
  close(mem_fd);

  if(gpio_map != MAP_FAILED)
    munmap(gpio_map, MAP_SIZE);

  return retval;
}


int main(int argc, char **argv)
{
  gpio_read();


  return 0;
}
