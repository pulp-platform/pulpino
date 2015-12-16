/**
 * @file
 * @brief 16750 UART library.
 *
 * Provides UART helper function like setting
 * control registers and reading/writing over
 * the serial interface.
 *
 * @author Unknown
 *
 * @version 1.0
 *
 * @date 2/10/2015
 *
 */
#ifndef _UART_H
#define _UART_H

#include "pulpino.h"
#include <stdint.h>

#define UART_REG_RBR ( UART_BASE_ADDR + 0x00) // Receiver Buffer Register (Read Only)
#define UART_REG_DLL ( UART_BASE_ADDR + 0x00) // Divisor Latch (LS)
#define UART_REG_THR ( UART_BASE_ADDR + 0x00) // Transmitter Holding Register (Write Only)
#define UART_REG_DLM ( UART_BASE_ADDR + 0x04) // Divisor Latch (MS)
#define UART_REG_IER ( UART_BASE_ADDR + 0x04) // Interrupt Enable Register
#define UART_REG_IIR ( UART_BASE_ADDR + 0x08) // Interrupt Identity Register (Read Only)
#define UART_REG_FCR ( UART_BASE_ADDR + 0x08) // FIFO Control Register (Write Only)
#define UART_REG_LCR ( UART_BASE_ADDR + 0x0C) // Line Control Register
#define UART_REG_MCR ( UART_BASE_ADDR + 0x10) // MODEM Control Register
#define UART_REG_LSR ( UART_BASE_ADDR + 0x14) // Line Status Register
#define UART_REG_MSR ( UART_BASE_ADDR + 0x18) // MODEM Status Register
#define UART_REG_SCR ( UART_BASE_ADDR + 0x1C) // Scratch Register

#define UART_FIFO_DEPTH 64

void uart_set_cfg(int parity, uint16_t clk_counter);

void uart_send(const char* str, unsigned int len);
void uart_sendchar(const char c);

char uart_getchar();

void uart_wait_tx_done(void);

#endif
