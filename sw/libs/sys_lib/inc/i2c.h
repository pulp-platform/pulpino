// Copyright 2016 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

/**
 * @file
 * @brief I2C Peripheral Library.
 *
 * I2C functions for sending and receiving data
 * over an I2C bus.
 *
 */
#ifndef _I2C_H_
#define _I2C_H_

#include <pulpino.h>

#define I2C_START       0x80
#define I2C_STOP        0x40
#define I2C_READ        0x20
#define I2C_WRITE       0x10
#define I2C_CLR_INT     0x01
#define I2C_START_READ  0xA0
#define I2C_STOP_READ   0x60
#define I2C_START_WRITE 0x90
#define I2C_STOP_WRITE  0x50

#define I2C_CTR_EN        0x80 // enable only
#define I2C_CTR_INTEN     0x40 // interrupt enable only
#define I2C_CTR_EN_INTEN  0xC0 // enable i2c and interrupts

#define I2C_REG_PRE             ( I2C_BASE_ADDR + 0x00 )
#define I2C_REG_CTR             ( I2C_BASE_ADDR + 0x04 )
#define I2C_REG_RX              ( I2C_BASE_ADDR + 0x08 )
#define I2C_REG_STATUS          ( I2C_BASE_ADDR + 0x0C )
#define I2C_REG_TX              ( I2C_BASE_ADDR + 0x10 )
#define I2C_REG_CMD             ( I2C_BASE_ADDR + 0x14 )

#define I2C_PRE 	REG(I2C_REG_PRE)             
#define I2C_CTR 	REG(I2C_REG_CTR)             
#define I2C_RX 		REG(I2C_REG_RX)              
#define I2C_STATUS 	REG(I2C_REG_STATUS)       
#define I2C_TX 		REG(I2C_REG_TX)              
#define I2C_CMD 	REG(I2C_REG_CMD)            

#define I2C_STATUS_RXACK 0x80
#define I2C_STATUS_BUSY  0x40
#define I2C_STATUS_AL    0x20
#define I2C_STATUS_TIP   0x02
#define I2C_STATUS_IF    0x01

/** Confgures I2C with the given prescaler.
 *
 * @param int prescaler - clock divider used to generate I2C clock.
 * @param int enable    - enable I2C peripheral
 *
*/
void i2c_setup(int prescaler,int enable);

/** Sends data via I2C.
 *
 * @param int value - data that should be transmitted.
 *
*/
void i2c_send_data(int value);

/** Send command to I2C.
 *
 * @param int value - command that should be send.
 *
*/
void i2c_send_command(int value);

/** Get status of the I2C peripheral.
 *
 * @return volatile int - status code
 *
*/
int  i2c_get_status(void);

/** Retrieves data from the I2C bus.
 *
 * @return volatile int - data received.
 *
 */
int  i2c_get_data(void);

/** Wait for acknowledge bit. Use after transmitting data.
 *
 *
 */
int  i2c_get_ack(void);

int i2c_busy(void);

#endif // _I2C_H_
