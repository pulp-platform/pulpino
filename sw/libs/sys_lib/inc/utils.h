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

#ifdef __riscv__
/** 
 * \brief Write to CSR.
 * \param CSR register to write.
 * \param Value to write to CSR register.
 * \return void
 *
 * Function to handle CSR writes.
 *
 */
#define csrw(csr, value)  asm volatile ("csrw\t\t" #csr ", %0" : /* no output */ : "r" (value));

/** 
 * \brief Read from CSR.
 * \param void
 * \return 32-bit unsigned int
 *
 * Function to handle CSR reads.
 *
 */
#define csrr(csr, value)  asm volatile ("csrr\t\t%0, " #csr "": "=r" (value)); 

#else // not __riscv__
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
