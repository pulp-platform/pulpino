// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

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
