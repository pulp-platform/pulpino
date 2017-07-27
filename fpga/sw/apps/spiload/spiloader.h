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

#ifndef SPILOADER_H
#define SPILOADER_H

#include <time.h>

static inline struct timespec timespec_sub(struct timespec lhs, struct timespec rhs) {
  struct timespec ret;

  if (rhs.tv_nsec > lhs.tv_nsec) {
    ret.tv_sec = lhs.tv_sec - 1 - rhs.tv_sec;
    ret.tv_nsec = 1000000000 + lhs.tv_nsec - rhs.tv_nsec;
  }
  else
  {
    ret.tv_sec = lhs.tv_sec - rhs.tv_sec;
    ret.tv_nsec = lhs.tv_nsec - rhs.tv_nsec;
  }

  return ret;
}

struct cmd_arguments_t {
  char* stim;
  unsigned int timeout;
};

void* console_thread(void* ptr);
void console_thread_stop();

#endif
