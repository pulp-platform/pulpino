/******************************************************************************
 *                                                                            *
 *          ERC Multitherman Lab @ DEI - University of Bologna                *
 *                      Viale Pepoli 3/2 - 40136                              *
 *                     Bologna - phone 0512092759                             *
 *                                                                            *
 * Engineer:       Francesco Conti - f.conti@unibo.it                         *
 *                                                                            *
 * Project:        Mia Wallace PULPv3 65nm chip                               *
 * File:           hwce.h                                                     *
 * Description:    HWCE hardware abstraction layer                            *
 *                                                                            *
 ******************************************************************************/

#ifndef	__HWCE_H__
#define __HWCE_H__

#include "pulp.h"
#include "utils.h"

#define N_MANDATORY_REGS   7
#define N_RESERVED_REGS    1
#define N_GENERIC_REGS     8
#define N_MAX_GENERIC_REGS 8

#define NB_HWCE 1

#define HWCE_Y_OUT 0
#define HWCE_Y_IN  1
#define HWCE_X_IN  2

/* LOW-LEVEL HAL */
#define HWCE_ADDR_BASE  HWCE_BASE_ADDR
#define HWCE_ADDR_SPACE 0x00000100

static inline void hwce_ll_hal_writecfg(unsigned int hwce_id, unsigned int reg, unsigned int datum) {
  *(volatile unsigned int *)(HWCE_ADDR_BASE + HWCE_ADDR_SPACE*hwce_id + (reg<<2)) = datum;
}

static inline unsigned int hwce_ll_hal_readcfg(unsigned int hwce_id, unsigned int reg) {
  return *(volatile unsigned int *)(HWCE_ADDR_BASE + HWCE_ADDR_SPACE*hwce_id + (reg<<2));
}

/* HWPE-like HAL */

// Triggers the HWCE execution
static inline void hwce_trigger(unsigned int hwce_id) {
   hwce_ll_hal_writecfg(hwce_id, 0,1);
}

// Reads status register, returns 1 if working, 0 if finished
static inline unsigned int hwce_status(unsigned int hwce_id) {
  return hwce_ll_hal_readcfg(hwce_id, 3);
}

// Reads ID of the running job
static inline unsigned int hwce_running_job(unsigned int hwce_id) {
  return hwce_ll_hal_readcfg(hwce_id, 4);
}

// Reads the context_id register and, if HW sync is active, initiates the setting phase
// returns -1 if no context available, or context_id if one is available
static inline unsigned int hwce_acquire(unsigned int hwce_id) {
  return hwce_ll_hal_readcfg(hwce_id, 1);
}

// Sets the synchronization mode (0=polling, 1=events)
static inline void hwce_set_sync(unsigned int hwce_id, unsigned int sync_mode) {
   hwce_ll_hal_writecfg(hwce_id, 2, sync_mode);
}

// Sets one of the generic registers (valid regs: 0 to N_GENERIC_REGS-1)
static inline void hwce_set_generic(unsigned int hwce_id, unsigned int reg, unsigned int value) {
  hwce_ll_hal_writecfg(hwce_id, N_MANDATORY_REGS+N_RESERVED_REGS + reg, value);
}

// Sets one of the IO registers (valid regs: 0 to N_IO_REGS-1)
static inline void hwce_set_IOReg(unsigned int hwce_id, unsigned int reg, unsigned int value) {
  hwce_ll_hal_writecfg(hwce_id, N_MANDATORY_REGS+N_RESERVED_REGS+N_MAX_GENERIC_REGS + reg, value);
}

// Sets the base_addr reg
static inline void hwce_set_base_addr(unsigned int hwce_id, unsigned char stream, unsigned int value) {
  unsigned int reg;
  reg = stream*4 + 0;
  hwce_set_IOReg(hwce_id, reg, value);
}

// Sets the trans_size reg
static inline void hwce_set_trans_size(unsigned int hwce_id, unsigned char stream, unsigned int value) {
  unsigned int reg;
  reg = stream*4 + 1;
  hwce_set_IOReg(hwce_id, reg, value);
}

// Sets the line_param reg (stride is in bytes)
static inline void hwce_set_line_params(unsigned int hwce_id, unsigned char stream, unsigned short int length, unsigned short int stride) {
  unsigned int reg;
  unsigned int value;
  reg = stream*4 + 2;
  value = (stride << 16) | length;
  hwce_set_IOReg(hwce_id, reg, value);
}

// Sets the feat_param reg (stride is in bytes)
static inline void hwce_set_feat_params(unsigned int hwce_id, unsigned char stream, unsigned short int length, unsigned short int stride) {
  unsigned int reg;
  unsigned int value;
  reg = stream*4 + 3;
  value = (stride << 16) | length;
  hwce_set_IOReg(hwce_id, reg, value);
}

static inline void hwce_setup(unsigned int hwce_id, unsigned int weight_ptr, unsigned int x_in_ptr, unsigned int y_in_ptr, unsigned int y_out_ptr, unsigned int n_feat, unsigned int height, unsigned int width_total, unsigned int width_hwce, short int fw_hw, short int fw_eff, short int mask) {
  // width is scaled down by a factor of two because image is 16-bit based
  int fw_hwce = fw_hw-1;
  hwce_set_IOReg       (hwce_id, 12, weight_ptr); // pointer to the weights array
  hwce_set_IOReg       (hwce_id, 13, (fw_eff << 16) | ((0x0000ffff) & mask)); // effective size of filter and number of mask
  hwce_set_IOReg       (hwce_id, 14, (n_feat << 16) | 13); // number of features, QF bit
  hwce_set_base_addr   (hwce_id, HWCE_Y_OUT, y_out_ptr);
  hwce_set_trans_size  (hwce_id, HWCE_Y_OUT, n_feat*(height-fw_hwce)*(width_hwce/2-fw_hwce/2));
  hwce_set_line_params (hwce_id, HWCE_Y_OUT, width_hwce/2-fw_hwce/2, 4*(width_total/2-fw_eff/2));
  hwce_set_feat_params (hwce_id, HWCE_Y_OUT, (height-fw_hwce), 4*(height-fw_hwce)*(width_total/2-fw_eff/2));
  hwce_set_base_addr   (hwce_id, HWCE_Y_IN,  y_in_ptr);
  hwce_set_trans_size  (hwce_id, HWCE_Y_IN,  n_feat*(height-fw_hwce)*(width_hwce/2-fw_hwce/2));
  hwce_set_line_params (hwce_id, HWCE_Y_IN,  width_hwce/2-fw_hwce/2, 4*(width_total/2-fw_eff/2));
  hwce_set_feat_params (hwce_id, HWCE_Y_IN,  (height-fw_hwce), 0); //4*(height-fw_hwce)*(width_total/2-fw_eff/2));
  hwce_set_base_addr   (hwce_id, HWCE_X_IN,  x_in_ptr);
  hwce_set_trans_size  (hwce_id, HWCE_X_IN,  n_feat*height*width_hwce/2);
  hwce_set_line_params (hwce_id, HWCE_X_IN,  width_hwce/2, 4*width_total/2);
  hwce_set_feat_params (hwce_id, HWCE_X_IN,  height, 4*height*width_total/2);
}

static inline int hwce_getnum() {
  return NB_HWCE;
}

/* Higher level HWCE APIs */

void hwce5x5_wait(int offload_id); 

void hwce5x5_conv16_11x11(short int **W, short int *x, short int *y, int h, int w, int nif, int a);
void hwce5x5_conv16_9x9(short int **W, short int *x, short int *y, int h, int w, int nif, int a);
void hwce5x5_conv16_7x7(short int **W, short int *x, short int *y, int h, int w, int nif, int a);
void hwce5x5_conv16_5x5(short int **W, short int *x, short int *y, int h, int w, int nif, int a);
void hwce5x5_conv16_3x3(short int **W, short int *x, short int *y, int h, int w, int nif, int a);

int hwce5x5_conv16_11x11_async(short int **W, short int *x, short int *y, int h, int w, int nif, int a);
int hwce5x5_conv16_9x9_async(short int **W, short int *x, short int *y, int h, int w, int nif, int a);
int hwce5x5_conv16_7x7_async(short int **W, short int *x, short int *y, int h, int w, int nif, int a);
int hwce5x5_conv16_5x5_async(short int **W, short int *x, short int *y, int h, int w, int nif, int a);
int hwce5x5_conv16_3x3_async(short int **W, short int *x, short int *y, int h, int w, int nif, int a);

int hwce5x5_conv16_5x5_nongreedy_async(short int **W, short int *x, short int *y, int h, int w, int nif, int a);
void hwce5x5_conv16_5x5_nongreedy(short int **W, short int *x, short int *y, int h, int w, int nif, int a);

#endif

