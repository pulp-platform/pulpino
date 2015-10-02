/**
 * @file
 * @brief Register mapping for PULPino peripherals.
 * 
 * Contains event register mappings for the PULPino SOC as
 * well as some general definitions for the overall system.
 *
 * @author Florian Zaruba
 *
 * @version 1.0
 *
 * @date 2/10/2015
 *
 */
#ifndef PULPINO_H
#define PULPINO_H

#define PULPINO_BASE_ADDR             0x10000000

/* SOC PERIPHERALS */
#define SOC_PERIPHERALS_BASE_ADDR     ( PULPINO_BASE_ADDR + 0xA100000 )

#define UART_BASE_ADDR                ( SOC_PERIPHERALS_BASE_ADDR + 0x0000 )
#define GPIO_BASE_ADDR                ( SOC_PERIPHERALS_BASE_ADDR + 0x1000 )
#define SPI_BASE_ADDR                 ( SOC_PERIPHERALS_BASE_ADDR + 0x2000 )
#define TIMER_BASE_ADDR               ( SOC_PERIPHERALS_BASE_ADDR + 0x3000 )
#define EVENT_UNIT_BASE_ADDR 		  ( SOC_PERIPHERALS_BASE_ADDR + 0x4000 )	

#define CLK_DIV_ADDR                  ( SOC_PERIPHERALS_BASE_ADDR + 0x4000 )

/* STDOUT */
#define STDOUT_BASE_ADDR              ( SOC_PERIPHERALS_BASE_ADDR + 0x10000 )
#define FPUTCHAR_BASE_ADDR            ( STDOUT_BASE_ADDR + 0x1000 )
#define FILE_CMD_BASE_ADDR            ( STDOUT_BASE_ADDR + 0x2000 )
#define STREAM_BASE_ADDR              ( STDOUT_BASE_ADDR + 0x3000 )

/* ROM */
#define ROM_BASE_ADDR                 ( PULP_BASE_ADDR + 0xA000000 )

#endif
