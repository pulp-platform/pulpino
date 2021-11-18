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
#include <linux/spi/spidev.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "spiloader.h"

#define SPIDEV               "/dev/spidev1.0"
#define CLKING_AXI_ADDR      0x51010000
#define PULP_CTRL_AXI_ADDR   0x51000000
#define PULP_GPIO_AXI_ADDR   0x51030000


#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

int clock_manager();
int process_file(char* buffer, size_t size);

char *map_device(char *base, uint32_t axi_addr) {
  // Map device only once
  if (base != NULL)
    return base;

  static int mem_fd = -1;
  if (mem_fd < 0) {
    if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      perror("Can't open /dev/mem");
      return NULL;
    }
  }

  char* map = (char*)mmap(
      NULL,
      MAP_SIZE,
      PROT_READ|PROT_WRITE,
      MAP_SHARED,
      mem_fd,
      axi_addr & ~MAP_MASK
  );
  if (map == MAP_FAILED) {
    perror("mmap error");
    return NULL;
  }

  return map + (axi_addr & MAP_MASK);
}

int pulp_ctrl(int fetch_en, int reset) {
  static char* ctrl_base = NULL;
  ctrl_base = map_device(ctrl_base, PULP_CTRL_AXI_ADDR);
  if (ctrl_base == NULL)
    return -1;

  volatile uint32_t* gpio = (volatile uint32_t*)(ctrl_base + 0x0);
  volatile uint32_t* dir  = (volatile uint32_t*)(ctrl_base + 0x4);

  // now we can actually write to the peripheral
  uint32_t val = 0x0;
  if (reset == 0)
    val |= (1 << 31); // reset is active low

  if (fetch_en)
    val |= (1 << 0);

  *dir  = 0x0; // configure as output
  *gpio = val;

  return 0;
}

int wait_eoc(unsigned int timeout) {
  static char* gpio_base = NULL;
  gpio_base = map_device(gpio_base, PULP_GPIO_AXI_ADDR);
  if (gpio_base == NULL)
    return -1;

  struct timespec spec_start, spec_end, spec_diff;

  volatile uint32_t* gpio = (volatile uint32_t*)(gpio_base + 0x0);
  volatile uint32_t* dir  = (volatile uint32_t*)(gpio_base + 0x4);

  // now we can actually write to the peripheral
  *dir  = 0xFFFFFFFF; // configure as input
  clock_gettime(CLOCK_REALTIME, &spec_start);
  // now wait for timeout

  while (1) {
    clock_gettime(CLOCK_REALTIME, &spec_end);
    spec_diff = timespec_sub(spec_end, spec_start);

    if (*gpio & (0x1 << 8)) {
      printf("EOC received!\n");
      break;
    }

    if (spec_diff.tv_sec >= timeout) {
      fprintf(stderr, "Timeout reached!\n");
      break;
    }
  }

  printf("Stopped after %ld.%ld\n", (long)spec_diff.tv_sec, spec_diff.tv_nsec);

  return 0;
}

int spi_read_reg(unsigned int addr) {
  int fd;
  char wr_buf[256];
  char rd_buf[256];
  unsigned int i;
  size_t transfer_len = 5;
  int retval = 0;


  // open spidev
  fd = open(SPIDEV, O_RDWR);
  if (fd <= 0) {
    perror(SPIDEV " not found");

    retval = -1;
    goto fail;
  }

  struct spi_ioc_transfer transfer = {
    .tx_buf        = 0,
    .rx_buf        = 0,
    .len           = 0,
    .delay_usecs   = 0,
    .speed_hz      = 0,
    .bits_per_word = 0,
  };

  transfer.tx_buf = (unsigned long)wr_buf;
  transfer.rx_buf = (unsigned long)rd_buf;
  transfer.len    = transfer_len;

  memset(wr_buf, 0, transfer.len);
  memset(rd_buf, 0, transfer.len);

  switch(addr) {
    case 0: wr_buf[0] = 0x05; break; // read reg0
    case 1: wr_buf[0] = 0x07; break; // read reg1
    case 2: wr_buf[0] = 0x21; break; // read reg2
    case 3: wr_buf[0] = 0x30; break; // read reg3
    default:
            fprintf(stderr, "Not a valid address for reading a register\n");
            goto fail;
  }


  // check if write was successful
  if (ioctl(fd, SPI_IOC_MESSAGE(1), &transfer) < 0) {
    perror("SPI_IOC_MESSAGE");
    retval = -1;
    goto fail;
  }

  for(i = 4; i < transfer.len; i++)
    printf("Got %X\n", rd_buf[i]);

fail:
  // close spidev
  close(fd);

  return retval;
}

int set_boot_addr(uint32_t boot_addr) {
  int fd;
  char wr_buf[9];
  int retval = 0;

  const uint32_t reg_addr = 0x1A107008;

  wr_buf[0] = 0x02; // write command
  wr_buf[1] = (reg_addr >> 24) & 0xFF;
  wr_buf[2] = (reg_addr >> 16) & 0xFF;
  wr_buf[3] = (reg_addr >>  8) & 0xFF;
  wr_buf[4] = (reg_addr >>  0) & 0xFF;
  // address
  wr_buf[5] = (boot_addr >> 24) & 0xFF;
  wr_buf[6] = (boot_addr >> 16) & 0xFF;
  wr_buf[7] = (boot_addr >>  8) & 0xFF;
  wr_buf[8] = (boot_addr >>  0) & 0xFF;

  // open spidev
  fd = open(SPIDEV, O_RDWR);
  if (fd <= 0) {
    perror(SPIDEV " not found");

    retval = -1;
    goto fail;
  }

  // write to spidev
  if (write(fd, wr_buf, 9) != 9) {
    perror("Could not write to " SPIDEV);

    retval = -1;
    goto fail;
  }

fail:
  // close spidev
  close(fd);

  return retval;
}

int spi_load(uint32_t addr, char* in_buf, size_t in_size) {
  int fd = -1;
  char* wr_buf = NULL;
  char* rd_buf = NULL;
  unsigned int i;
  size_t size;
  size_t transfer_len;
  int retval = 0;

  // make sure transfers are 32 bit aligned
  if ((in_size & 0x3) == 0)
    size = in_size;
  else
    size = (in_size & (~0x3)) + 0x4;

  transfer_len = size + 9 + 4 + 4;

  wr_buf = (char*)malloc(transfer_len);
  if (wr_buf == NULL) {
    fprintf(stderr, "Unable to acquire write buffer\n");

    retval = -1;
    goto fail;
  }

  memset(wr_buf, 0, transfer_len);

  wr_buf[0] = 0x02; // write command
  // address
  wr_buf[1] = (addr >> 24) & 0xFF;
  wr_buf[2] = (addr >> 16) & 0xFF;
  wr_buf[3] = (addr >>  8) & 0xFF;
  wr_buf[4] = (addr >>  0) & 0xFF;

  memcpy(wr_buf + 5, in_buf, in_size);

  // open spidev
  fd = open(SPIDEV, O_RDWR);
  if (fd <= 0) {
    perror(SPIDEV " not found");

    retval = -1;
    goto fail;
  }

  // write to spidev
  if (write(fd, wr_buf, size + 5) != (size + 5)) {
    perror("Could not write to " SPIDEV);

    retval = -1;
    goto fail;
  }


  // prepare for readback
  rd_buf = (char*)malloc(transfer_len);
  if (rd_buf == NULL) {
    fprintf(stderr, "Unable to acquire buffer to check if write was successful\n");

    retval = -1;
    goto fail;
  }

  struct spi_ioc_transfer transfer = {
    .tx_buf        = 0,
    .rx_buf        = 0,
    .len           = 0,
    .delay_usecs   = 0,
    .speed_hz      = 0,
    .bits_per_word = 0,
  };

  transfer.tx_buf = (unsigned long)wr_buf;
  transfer.rx_buf = (unsigned long)rd_buf;
  transfer.len    = transfer_len;

  memset(wr_buf, 0, transfer.len);
  memset(rd_buf, 0, transfer.len);

  wr_buf[0] = 0x0B; // read command
  // address
  wr_buf[1] = (addr >> 24) & 0xFF;
  wr_buf[2] = (addr >> 16) & 0xFF;
  wr_buf[3] = (addr >>  8) & 0xFF;
  wr_buf[4] = (addr >>  0) & 0xFF;

  // check if write was successful
  if (ioctl(fd, SPI_IOC_MESSAGE(1), &transfer) < 0) {
    perror("SPI_IOC_MESSAGE");
    retval = -1;
    goto fail;
  }

  // shift everything by one bit
  for(i = 0; i < transfer.len-1; i++) {
    rd_buf[i] = (rd_buf[i] << 1) | ((rd_buf[i+1] & 0x80) >> 7);
  }

  for(i = 0; i < in_size; i++) {
    if (in_buf[i] != rd_buf[i + 9]) {
      fprintf(stderr, "Read check failed at idx %d: Expected %02X, got %02X\n", i, in_buf[i], rd_buf[i + 9]);
    }
  }

fail:
  // close spidev if opened
  if (fd > 0)
    close(fd);

  free(wr_buf);
  free(rd_buf);

  return retval;
}

int read_file(const char *path, char **bufp, size_t *sizep) {
  int fd = open(path, O_RDWR);
  if (fd <= 0) {
    fprintf(stderr, "Could not open \"%s\": %s\n", path, strerror(errno));
    return 1;
  }

  *sizep = lseek(fd, 0, SEEK_END);
  if (*sizep == -1) {
    fprintf(stderr, "Could not determine size of \"%s\": %s\n", path, strerror(errno));
    return 1;
  }

  *bufp = malloc(*sizep);
  if(*bufp == NULL) {
    fprintf(stderr, "Could not allocate memory to buffer \"%s\".\n", path);
    return 1;
  }

  if (lseek(fd, 0, SEEK_SET) == -1) {
    fprintf(stderr, "Could not jump to start of file \"%s\": %s\n", path, strerror(errno));
    return 1;
  }

  if (read(fd, *bufp, *sizep) != *sizep) {
    fprintf(stderr, "Could not read from file \"%s\": %s\n", path, strerror(errno));
    close(fd);
    return 1;
  }

  return 0;
}

int main(int argc, char **argv)
{
  struct cmd_arguments_t arguments;

  cmd_parsing(argc, argv, &arguments);

  if (clock_manager() != 0) {
    fprintf(stderr, "Could not configure clock.\n");
    return 1;
  }

  // reset target
  if (pulp_ctrl(0, 1) || pulp_ctrl(0, 0)) {
    fprintf(stderr, "Could not reset target.\n");
    return 1;
  }

  printf("Target is in reset\n");

  if (!arguments.reset) {
    char* buffer;
    size_t size;
    if (read_file(arguments.stim, &buffer, &size) != 0) {
      fprintf(stderr, "Could not read in stim file.\n");
      return 1;
    }
    if (process_file(buffer, size) != 0) {
      fprintf(stderr, "Could not process stim file.\n");
      return 1;
    }
    free(buffer);
  }

  // Start target and wait for timeout (if any)
  if (set_boot_addr(0x00000000) != 0) {
      fprintf(stderr, "Could not set PC.\n");
      return 1;
  }

  if (arguments.timeout > 0) {
    console_thread_start();
    sleep(1);
  }

  printf("Starting target\n");
  if (pulp_ctrl(1, 0) != 0) {
      fprintf(stderr, "Could not (re)start target.\n");
      return 1;
  }

  if (arguments.timeout > 0) {
    printf("Waiting for EOC...\n");

    wait_eoc(arguments.timeout);

    // wait for a moment to also let UART communication finish
    sleep(1);
    console_thread_stop();
  }

  return 0;
}

#define NUM_ENTRIES  32768

int process_file(char* buffer, size_t size) {
  uint32_t addr[NUM_ENTRIES];
  uint32_t data[NUM_ENTRIES];
  unsigned int entries = 0;

  // extract lines
  const char *const buffer_end = buffer + size;
  char line[20];
  unsigned int i;

  while(buffer != buffer_end) {
    // build lines
    i = 0;
    while (buffer != buffer_end) {
      line[i] = *buffer++;

      if (line[i] == '\n') {
        line[i] = '\0';
        break;
      }

      if (buffer == buffer_end) {
        line[i+1] = '\0';
        break;
      }

      i++;
      if (i == 18) {
        fprintf(stderr, "Failed to parse, couldn't find line\n");
        return -1;
      }
    }

    sscanf(line, "%X_%X", &addr[entries], &data[entries]);

    // convert data
    data[entries] = __bswap_32(data[entries]);
    //    printf("Line %s, addr %08X, data %08X\n", line, addr[entries], data[entries]);

    entries++;

    if (entries == NUM_ENTRIES) {
      fprintf(stderr, "Too many entries in file\n");
      return -1;
    }
  }

  if (entries == 0) {
    fprintf(stderr, "No entries found\n");
    return -1;
  }

  // now find consecutive addresses to build blocks
  unsigned int start_idx = 0;
  for(i = 1; i < entries; i++) {
    if(addr[i] != (addr[i-1] + 0x4) || (i - start_idx) == 255 || i == (entries - 1)) {
      // send block
      printf("Sending block addr %08X with %d entries\n", addr[start_idx], i - start_idx + 1);
      if (spi_load(addr[start_idx], (char*)&data[start_idx], (i - start_idx + 1) * 4) != 0) {
        fprintf(stderr, "Sending block failed!\n");
        return -1;
      }
      start_idx = i;
    }
  }

  return 0;
}

int clock_manager() {
  static char* clk_base = NULL;
  clk_base = map_device(clk_base, CLKING_AXI_ADDR);
  if (clk_base == NULL)
    return -1;

  // volatile uint32_t* sr = (volatile uint32_t*)(clk_base + 0x4);
  volatile uint32_t* ccr0  = (volatile uint32_t*)(clk_base + 0x200);
  volatile uint32_t* ccr2  = (volatile uint32_t*)(clk_base + 0x208);

  /* printf("SR   is %08X\n", *sr); */
  /* printf("CCR0 is %08X\n", *ccr0); */
  /* printf("CCR2 is %08X\n", *ccr2); */

  // set to 5 MHz
  *ccr0 = 0x04004005;
  *ccr2 = 0x00040080;

  return 0;
}
