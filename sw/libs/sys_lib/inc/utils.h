#ifndef _UTILS_H_
#define _UTILS_H_

/* For getting core ID. */
static inline int get_core_id()
{
  return 0;
}

// get number of cores
static inline int get_core_num()
{
  return 0;
}


#ifndef __riscv__
/* For writing into SPR. */
static inline void mtspr(unsigned long spr, unsigned long value)
{
  asm volatile ("l.mtspr\t\t%0,%1,0": : "r" (spr), "r" (value));
}

/* For reading SPR. */
static inline unsigned long mfspr(unsigned long spr)
{
  unsigned long value;
  asm volatile ("l.mfspr\t\t%0,%1,0" : "=r" (value) : "r" (spr));
  return value;
}
#endif

/* Loops/exits simulation */
void exit(int i);

/* end of computation */
void eoc(int i);

// sleep some cycles
void sleep(volatile int);


#endif
