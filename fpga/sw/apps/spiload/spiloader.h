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
