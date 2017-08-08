// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

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

