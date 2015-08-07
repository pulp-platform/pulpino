#ifndef _UTILS_H_
#define _UTILS_H_

/* For getting core ID. */
int get_core_id();

/* For getting core ID. */
int get_cluster_id();

int get_core_num();

/* For writing into SPR. */
void mtspr(unsigned long spr, unsigned long value);

/* For reading SPR. */
unsigned long mfspr(unsigned long spr);

/* Loops/exits simulation */
void exit(int i);

/* end of computation */
void eoc(int i);

// sleep some cycles
void sleep(volatile int);


#endif
