#ifndef _UART_H
#define _UART_H

#include "pulpino.h"
#include <stdint.h>

#define UART_REG_RBR ( UART_BASE_ADDR + 0x00)
#define UART_REG_DLL ( UART_BASE_ADDR + 0x00)
#define UART_REG_THR ( UART_BASE_ADDR + 0x00)
#define UART_REG_DLM ( UART_BASE_ADDR + 0x04)
#define UART_REG_IER ( UART_BASE_ADDR + 0x04)
#define UART_REG_IIR ( UART_BASE_ADDR + 0x08)
#define UART_REG_FCR ( UART_BASE_ADDR + 0x08)
#define UART_REG_LCR ( UART_BASE_ADDR + 0x0C)
#define UART_REG_MCR ( UART_BASE_ADDR + 0x10)
#define UART_REG_LSR ( UART_BASE_ADDR + 0x14)
#define UART_REG_MSR ( UART_BASE_ADDR + 0x18)
#define UART_REG_SCR ( UART_BASE_ADDR + 0x1C)

#define UART_FIFO_DEPTH 64

void uart_set_cfg(int parity, uint16_t clk_counter);

void uart_send(const char* str, unsigned int len);
void uart_sendchar(const char c);

char uart_getchar();

void uart_wait_tx_done(void);


extern volatile char lock_uart_lock;

#define uart_lock() while(lock_uart_lock != 0)
#define uart_unlock() lock_uart_lock = 0

#endif
