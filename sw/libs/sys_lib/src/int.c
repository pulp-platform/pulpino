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

#include "utils.h"
#include "int.h"
#include "string_lib.h"
#include "uart.h"
#include "event.h"


//defining all interrupt handelrs
//these functions can be redefined by users

// 23: i2c
__attribute__ ((weak))
void ISR_I2C (void){ for(;;); }	

// 24: uart
__attribute__ ((weak))
void ISR_UART (void){ for(;;); }	

// 25: gpio
__attribute__ ((weak))
void ISR_GPIO (void){ for(;;); }	

// 26: spim end of transmission
__attribute__ ((weak))
void ISR_SPIM0 (void){ for(;;); }  

// 27: spim R/T finished
__attribute__ ((weak))
void ISR_SPIM1 (void){ for(;;); }  

// 28: timer A overflow
__attribute__ ((weak))
void ISR_TA_OVF (void){ for(;;); } 

// 29: timer A compare
__attribute__ ((weak))
void ISR_TA_CMP (void){ for(;;); } 

// 30: timer B overflow
__attribute__ ((weak))
void ISR_TB_OVF (void){ for(;;); } 

// 31: timer B compare
__attribute__ ((weak))
void ISR_TB_CMP (void){ for(;;); }

