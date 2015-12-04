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
#define EVENT_UNIT_BASE_ADDR          ( SOC_PERIPHERALS_BASE_ADDR + 0x4000 )
#define I2C_BASE_ADDR                 ( SOC_PERIPHERALS_BASE_ADDR + 0x5000 )
#define FLL_BASE_ADDR                 ( SOC_PERIPHERALS_BASE_ADDR + 0x6000 )
#define SOC_CTRL_BASE_ADDR            ( SOC_PERIPHERALS_BASE_ADDR + 0x7000 )

/* STDOUT */
#define STDOUT_BASE_ADDR              ( SOC_PERIPHERALS_BASE_ADDR + 0x10000 )
#define FPUTCHAR_BASE_ADDR            ( STDOUT_BASE_ADDR + 0x1000 )
#define FILE_CMD_BASE_ADDR            ( STDOUT_BASE_ADDR + 0x2000 )
#define STREAM_BASE_ADDR              ( STDOUT_BASE_ADDR + 0x3000 )

/* Instruction RAM */
#define INSTR_RAM_BASE_ADDR           ( 0x80       )

/* ROM */
#define ROM_BASE_ADDR                 ( 0x8000     )

/* Data RAM */
#define DATA_RAM_BASE_ADDR            ( 0x00100000 )

#endif
