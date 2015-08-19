/******************************************************************************
 *                                                                            *
 *          ERC Multitherman Lab @ DEI - University of Bologna                *
 *                      Viale Pepoli 3/2 - 40136                              *
 *                     Bologna - phone 0512092759                             *
 *                                                                            *
 * Engineer:       Francesco Conti - f.conti@unibo.it                         *
 *                                                                            *
 * Project:        Mia Wallace PULPv3 65nm chip                               *
 * File:           hwce.c                                                     *
 * Description:    HWCE hardware abstraction layer                            *
 *                                                                            *
 ******************************************************************************/

#include "hwce.h"
#include "events.h"
#include "events_ids.h"

// #define DEBUG

/* High-level HAL */

// waits on an offloaded job
inline void hwce_wait_on_job(unsigned hwce_id, unsigned int job_id) {
  while(hwce_running_job(hwce_id) <= job_id);
}

// waits on an offloaded job
inline void hwce_wait_on_job_event(unsigned hwce_id, unsigned int job_id) {

  int coreID = get_core_id();
  
  set_evnt_mask_low( coreID, read_evnt_mask_low(coreID) | 1 << EVENT_HWCE );

  while(hwce_running_job(hwce_id) <= job_id){
    wait_event();
    clear_evnt_buff_low(1 << EVENT_HWCE);
  }
    
}

// offload of jobs with non-greedy algorithm (doesn't work with shift-reg HWCE)
int hwce_nongreedy_offload(short int *W_ptr, short int *x_ptr, short int *y_ptr, unsigned int fw_hw, unsigned int fw_eff, unsigned int h, unsigned int w, unsigned int nif, unsigned int ow, unsigned short int mask) {

  unsigned int ow_int = ow;
  int offload_id;
  unsigned int hwce_id = 0;
  short int *x_ptr_int = x_ptr;
  short int *y_ptr_int = y_ptr;

  // nongreedy algorithm for HWCE offload
  while (ow_int>0) {

    unsigned int ow_hwce = (ow_int < (32-fw_hw+1)) ? ow_int : (32-fw_hw+1);
    //if (ow_hwce % 2 != 0)
    //   ow_hwce++; // for now, just operate on even-wide images

    int offload_id_tmp = -1;

    ow_int -= ow_hwce;

    // busy-wait on all HWCEs
    while((offload_id_tmp = hwce_acquire(hwce_id)) < 0);
    offload_id = offload_id_tmp;

#ifdef DEBUG
    printf("   HWCE offload: W=%08x, x=%08x, y=%08x, w=%d\n", W_ptr, x_ptr_int, y_ptr_int, ow_hwce+fw_hw-1);
    printf("       ow_int=%d, ow_hwce=%d, fw_hw=%d, fw_eff=%d\n", ow_int, ow_hwce, fw_hw, fw_eff);
#endif
    // offload job to HWCE
    hwce_setup(hwce_id,
               (unsigned int) W_ptr,
               (unsigned int) x_ptr_int,
               (unsigned int) y_ptr_int,
               (unsigned int) y_ptr_int,
               nif,
               h,
               w,
               ow_hwce+fw_hw-1,
               fw_hw,
               fw_eff,
               mask);
    // trigger HWCE
    hwce_trigger(hwce_id);

    x_ptr_int += ow_hwce;
    y_ptr_int += ow_hwce;

  }

  // return last offload id
  return offload_id;

}

// offload of jobs with greedy algorithm; returns last job id
int hwce_greedy_offload(short int *W_ptr, short int *x_ptr, short int *y_ptr, unsigned int fw_hw, unsigned int fw_eff, unsigned int h, unsigned int w, unsigned int nif, unsigned int ow, unsigned short int mask) {

  unsigned int ow_int = ow;
  int offload_id;
  unsigned int hwce_id = 0;
  short int *x_ptr_int = x_ptr;
  short int *y_ptr_int = y_ptr;

  // greedy algorithm for HWCE offload
  while (ow_int>0) {

    unsigned int ow_hwce = (ow_int < 2) ? 1 : (ow_int < 4) ? 2 : (ow_int < 8) ? 4 : (ow_int < 16) ? 8 : (ow_int < 32) ? 16 : 32;

    int offload_id_tmp = -1;

    ow_int -= ow_hwce;

    // busy-wait on all HWCEs
    while((offload_id_tmp = hwce_acquire(hwce_id)) < 0);
    offload_id = offload_id_tmp;

#ifdef DEBUG
    printf("   HWCE offload: W=%08x, x=%08x, y=%08x, w=%d\n", W_ptr, x_ptr_int, y_ptr_int, ow_hwce+fw_hw-1);
    printf("       ow_int=%d, ow_hwce=%d, fw_hw=%d, fw_eff=%d\n", ow_int, ow_hwce, fw_hw, fw_eff);
#endif
    // offload job to HWCE
    hwce_setup(hwce_id,
               (unsigned int) W_ptr,
               (unsigned int) x_ptr_int,
               (unsigned int) y_ptr_int,
               (unsigned int) y_ptr_int,
               nif,
               h,
               w,
               ow_hwce+fw_hw-1,
               fw_hw,
               fw_eff,
               mask);
    // trigger HWCE
    hwce_trigger(hwce_id);

    x_ptr_int += ow_hwce;
    y_ptr_int += ow_hwce;

  }

  // return last offload id
  return offload_id;

}

/* Filter-size (5x5) specific HWCE calls */

// wait HWCE 0 only
void hwce5x5_wait(int offload_id) {

  unsigned int hwce_id = 0;
  //  hwce_wait_on_job(hwce_id, offload_id);
  hwce_wait_on_job_event(hwce_id, offload_id);

}

// synchronous convolutions (i.e. function call-like semantics)
void hwce5x5_conv16_11x11(short int **W, short int *x, short int *y, int h, int w, int nif, int a) {
  
  int offload_id = hwce5x5_conv16_11x11_async(W, x, y, h, w, nif, a);
  hwce5x5_wait(offload_id);

}

void hwce5x5_conv16_9x9(short int **W, short int *x, short int *y, int h, int w, int nif, int a) {
  
  int offload_id = hwce5x5_conv16_9x9_async(W, x, y, h, w, nif, a);
  hwce5x5_wait(offload_id);

}

void hwce5x5_conv16_7x7(short int **W, short int *x, short int *y, int h, int w, int nif, int a) {
  
  int offload_id = hwce5x5_conv16_7x7_async(W, x, y, h, w, nif, a);
  hwce5x5_wait(offload_id);

}

void hwce5x5_conv16_5x5_nongreedy(short int **W, short int *x, short int *y, int h, int w, int nif, int a) {
  
  int offload_id = hwce5x5_conv16_5x5_nongreedy_async(W, x, y, h, w, nif, a);
  hwce5x5_wait(offload_id);

}
void hwce5x5_conv16_5x5(short int **W, short int *x, short int *y, int h, int w, int nif, int a) {
  
  int offload_id = hwce5x5_conv16_5x5_async(W, x, y, h, w, nif, a);
  hwce5x5_wait(offload_id);

}

void hwce5x5_conv16_3x3(short int **W, short int *x, short int *y, int h, int w, int nif, int a) {
  
  int offload_id = hwce5x5_conv16_3x3_async(W, x, y, h, w, nif, a);
  hwce5x5_wait(offload_id);

}

// asynchronous convolutions (i.e. DMA transfer-like semantics)
int hwce5x5_conv16_11x11_async(short int **W, short int *x, short int *y, int h, int w, int nif, int a) {

  int offload_id;
  unsigned int hwce_id = 0;
  unsigned int oh = h - 10;
  unsigned int ow = w - 10;

  offload_id = hwce_greedy_offload(W[0]+a*nif*5*5, &x[0*w+0], y + a*oh*ow, 5, 11, h, w, nif, ow, 0);
  offload_id = hwce_greedy_offload(W[1]+a*nif*5*5, &x[0*w+3], y + a*oh*ow, 5, 11, h, w, nif, ow, 1);
  offload_id = hwce_greedy_offload(W[2]+a*nif*5*5, &x[0*w+6], y + a*oh*ow, 5, 11, h, w, nif, ow, 2);
  offload_id = hwce_greedy_offload(W[3]+a*nif*5*5, &x[3*w+0], y + a*oh*ow, 5, 11, h, w, nif, ow, 3);
  offload_id = hwce_greedy_offload(W[4]+a*nif*5*5, &x[3*w+3], y + a*oh*ow, 5, 11, h, w, nif, ow, 4);
  offload_id = hwce_greedy_offload(W[5]+a*nif*5*5, &x[3*w+6], y + a*oh*ow, 5, 11, h, w, nif, ow, 5);
  offload_id = hwce_greedy_offload(W[6]+a*nif*5*5, &x[6*w+0], y + a*oh*ow, 5, 11, h, w, nif, ow, 6);
  offload_id = hwce_greedy_offload(W[7]+a*nif*5*5, &x[6*w+3], y + a*oh*ow, 5, 11, h, w, nif, ow, 7);
  offload_id = hwce_greedy_offload(W[8]+a*nif*5*5, &x[6*w+6], y + a*oh*ow, 5, 11, h, w, nif, ow, 8);

  return offload_id;

}

int hwce5x5_conv16_9x9_async(short int **W, short int *x, short int *y, int h, int w, int nif, int a) {

  int offload_id;
  unsigned int hwce_id = 0;
  unsigned int oh = h - 8;
  unsigned int ow = w - 8;

  offload_id = hwce_greedy_offload(W[0]+a*nif*5*5, &x[0*w+0], y + a*oh*ow, 5, 9, h, w, nif, ow, 0);
  offload_id = hwce_greedy_offload(W[1]+a*nif*5*5, &x[0*w+4], y + a*oh*ow, 5, 9, h, w, nif, ow, 1);
  offload_id = hwce_greedy_offload(W[2]+a*nif*5*5, &x[4*w+0], y + a*oh*ow, 5, 9, h, w, nif, ow, 2);
  offload_id = hwce_greedy_offload(W[3]+a*nif*5*5, &x[4*w+4], y + a*oh*ow, 5, 9, h, w, nif, ow, 3);

  return offload_id;

}

int hwce5x5_conv16_7x7_async(short int **W, short int *x, short int *y, int h, int w, int nif, int a) {

  int offload_id;
  unsigned int hwce_id = 0;
  unsigned int oh = h - 6;
  unsigned int ow = w - 6;

  offload_id = hwce_greedy_offload(W[0]+a*nif*5*5, &x[0*w+0], y + a*oh*ow, 5, 7, h, w, nif, ow, 0);
  offload_id = hwce_greedy_offload(W[1]+a*nif*5*5, &x[0*w+2], y + a*oh*ow, 5, 7, h, w, nif, ow, 1);
  offload_id = hwce_greedy_offload(W[2]+a*nif*5*5, &x[2*w+0], y + a*oh*ow, 5, 7, h, w, nif, ow, 2);
  offload_id = hwce_greedy_offload(W[3]+a*nif*5*5, &x[2*w+2], y + a*oh*ow, 5, 7, h, w, nif, ow, 3);

  return offload_id;

}

int hwce5x5_conv16_5x5_async(short int **W, short int *x, short int *y, int h, int w, int nif, int a) {

  int offload_id;
  unsigned int hwce_id = 0;
  unsigned int oh = h - 4;
  unsigned int ow = w - 4;

  offload_id = hwce_greedy_offload(W[0]+a*nif*5*5, &x[0*w+0], y + a*oh*ow, 5, 5, h, w, nif, ow, 0);

  return offload_id;

}

int hwce5x5_conv16_5x5_nongreedy_async(short int **W, short int *x, short int *y, int h, int w, int nif, int a) {

  int offload_id;
  unsigned int hwce_id = 0;
  unsigned int oh = h - 4;
  unsigned int ow = w - 4;

  offload_id = hwce_nongreedy_offload(W[0]+a*nif*5*5, &x[0*w+0], y + a*oh*ow, 5, 5, h, w, nif, ow, 0);

  return offload_id;

}

int hwce5x5_conv16_3x3_async(short int **W, short int *x, short int *y, int h, int w, int nif, int a) {

  int offload_id;
  unsigned int hwce_id = 0;
  unsigned int oh = h+2 - 4;
  unsigned int ow = w+2 - 4;

  offload_id = hwce_greedy_offload(W[0]+a*nif*5*5, &x[0*(w+2)+0], y + a*oh*ow, 5, 5, h+2, w+2, nif, ow, 0);

  return offload_id;

}


