#ifndef MCHAN_H
#define MCHAN_H

#include "pulp.h"

#define TCDM_ADDR_REG      ( MCHAN_BASE_ADDR + 0x0 )
#define EXT_ADDR_REG       ( MCHAN_BASE_ADDR + 0x4 )
#define CMD_QUEUE          ( MCHAN_BASE_ADDR + 0x8 )
#define CMD_QUEUE_BUSY_REG ( MCHAN_BASE_ADDR + 0xC )

#define ST1     0x00
#define ST2     0x02
#define ST4     0x04
#define ST8     0x06
#define ST16    0x08
#define ST32    0x0A
#define ST64    0x0C
#define ST128   0x0E
#define ST256   0x10
#define ST512   0x12
#define ST1024  0x14
#define ST2048  0x16
#define ST4096  0x18
#define ST8192  0x1A
#define ST16384 0x1C
#define ST32768 0x1E

#define LD1     0x01
#define LD2     0x03
#define LD4     0x05
#define LD8     0x07
#define LD16    0x09
#define LD32    0x0B
#define LD64    0x0D
#define LD128   0x0F
#define LD256   0x11
#define LD512   0x13
#define LD1024  0x15
#define LD2048  0x17
#define LD4096  0x19
#define LD8192  0x1B
#define LD16384 0x1D
#define LD32768 0x1F

#define set_ext_addr(addr)		        \
  *(volatile int*)EXT_ADDR_REG = addr;

#define set_tcdm_addr(addr)			\
  *(volatile int*)TCDM_ADDR_REG = addr;

#define push_cmd(cmd)				\
  *(volatile int*)CMD_QUEUE = cmd;

#define dma_barrier()				\
    while(*(volatile int*)CMD_QUEUE_BUSY_REG); 

#endif
