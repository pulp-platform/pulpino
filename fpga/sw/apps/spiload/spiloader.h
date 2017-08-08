// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

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
