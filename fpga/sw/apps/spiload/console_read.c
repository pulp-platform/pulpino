// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include <asm/termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>

pthread_t g_thread;
int g_should_exit;

void read_port()
{
  char buffer[256];
  struct termios2 tio;
  int fd;
  unsigned int i;
  int n;
  char c;

#ifdef ZYBO
  if ((fd = open("/dev/ttyPS1", O_RDONLY | O_NOCTTY) ) < 0) {
    perror("open_port: Unable to open /dev/ttyPS0");
    return;
  }
#else
  if ((fd = open("/dev/ttyPS0", O_RDONLY | O_NOCTTY) ) < 0) {
    perror("open_port: Unable to open /dev/ttyPS0");
    return;
  }
#endif

  // set baudrate
  ioctl(fd, TCGETS2, &tio);
  tio.c_cflag &= ~CBAUD;
  tio.c_cflag |= BOTHER;
  tio.c_ispeed = 156250;
  tio.c_ospeed = 156250;

  if (ioctl(fd, TCSETS2, &tio) != 0) {
    perror("ioctl failed to set baudrate");
    return;
  }


  i = 0;
  while (!g_should_exit) {
    int n = read(fd, &c, 1);

    if (n == EINTR) {
      // we were interrupted, so we assume we should exit
      break;

    } else if (n < 0) {
      printf("read failed!\n");

    } else if (n > 0) {
      if (i == (sizeof(buffer) - 1) || c == '\n') {
        buffer[i] = '\0';
        printf("PULPino: %s\n", buffer);
        i = 0;
      } else {
        buffer[i] = c;
        i++;
      }
    }
  }

  close(fd);
}

void* console_thread(void* ptr) {
  read_port();
}

void console_thread_start() {
  g_should_exit = 0;
  if (pthread_create (&g_thread, NULL, console_thread, NULL) ) {
    printf("Error creating console listening thread\n");
  }
}

void console_thread_stop() {
  g_should_exit = 1;
  pthread_kill(g_thread, SIGINT);
  pthread_join(g_thread, NULL);
}
