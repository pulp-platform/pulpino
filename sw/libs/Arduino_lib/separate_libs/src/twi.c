/*
  twi.c - TWI/I2C library for Wiring & Arduino
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
  Modified 29 July 2016 by Mahmoud Elmohr       (Ported to RISC-V PULPino)
*/

#include <math.h>
#include <stdlib.h>
#include <twi.h>
#include "Arduino.h"

#ifndef cbi
#define cbi(sfr, bit) ((sfr) &= ~(bit))
#endif

#ifndef sbi
#define sbi(sfr, bit) ((sfr) |= (bit))
#endif

#include "pins_arduino.h"

static volatile uint8_t twi_state;
static volatile uint8_t twi_sendStop;			// should the transaction end with a stop
static volatile uint8_t twi_inRepStart;			// in the middle of a repeated start

static uint8_t twi_masterBuffer[TWI_BUFFER_LENGTH];
static volatile uint8_t twi_masterBufferIndex;
static volatile uint8_t twi_masterBufferLength;

static uint8_t twi_txBuffer[TWI_BUFFER_LENGTH];
static volatile uint8_t twi_txBufferIndex;
static volatile uint8_t twi_txBufferLength;

static uint8_t twi_rxBuffer[TWI_BUFFER_LENGTH];
static volatile uint8_t twi_rxBufferIndex;

static volatile uint8_t twi_error;
static volatile uint8_t Address_ACK;

/*
 * Function twi_init
 * Desc     readys twi pins and sets twi bitrate
 * Input    none
 * Output   none
 */
void twi_init(void)
{
  // initialize state
  twi_state = TWI_READY;
  twi_sendStop = true;		// default value
  twi_inRepStart = false;

  // activate internal pullups for twi. 	###Check Later### mux

  // initialize twi prescaler and bit rate
  I2C_PRE = (uint16_t)(F_CPU / TWI_FREQ);
  // enable twi
  I2C_CTR = I2C_CTR_EN;
  IER |= (1<<23);	//enable I2C interrupt for PULPino event handler
}

/*
 * Function twi_disable
 * Desc     disables twi pins
 * Input    none
 * Output   none
 */
void twi_disable(void)
{
  // disable twi module and twi interrupt
  I2C_CTR = 0x00;
  IER &= ~(1<<23);	//enable Timer A compare match interrupt

  // deactivate internal pullups for twi.	###Check Later### mux

}


/*
 * Function twi_setClock
 * Desc     sets twi bit rate
 * Input    Clock Frequency
 * Output   none
 */
void twi_setFrequency(uint32_t frequency)
{
  I2C_PRE = (uint16_t)(F_CPU / frequency);

}

/*
 * Function twi_readFrom	##Check Later ###
 * Desc     attempts to become twi bus master and read a
 *          series of bytes from a device on the bus
 * Input    address: 7bit i2c device address
 *          data: pointer to byte array
 *          length: number of bytes to read into array
 *          sendStop: Boolean indicating whether to send a stop at the end
 * Output   number of bytes read
 */
uint8_t twi_readFrom(uint8_t address, uint8_t* data, uint8_t length, uint8_t sendStop)
{
  uint8_t i;

  // ensure data will fit into buffer
  if(TWI_BUFFER_LENGTH < length){
    return 1;
  }

  if(TWI_READY != twi_state){
    twi_error = TW_BUS_ERROR;	//bus error
    return 4;
  }

   // wait until bus is not busy
  while((I2C_STATUS & I2C_STATUS_BUSY) == I2C_STATUS_BUSY ){
    continue;
  }

  twi_sendStop = sendStop;

  // reset error state (0xFF.. no error occurred)
  twi_error = 0xFF;

  // initialize buffer iteration vars
  twi_masterBufferIndex = 0;
  twi_masterBufferLength = length;  // This is not intuitive, read on...			### Check Later ###
  // On receive, the previously configured ACK/NACK setting is transmitted in
  // response to the received byte before the interrupt is signalled.
  // Therefor we must actually set NACK when the _next_ to last byte is
  // received, causing that NACK to be sent in response to receiving the last
  // expected byte of data.

  // put address in the TX
  I2C_TX  = (address <<1) | 0x01;	//Device address should be only 7 bits the 8th bit determine read or write (here read as one)
  I2C_CTR =  I2C_CTR_EN_INTEN;	//Enable interrupt

  if (true == twi_inRepStart) {	//###Check Later ###
    // if we're in the repeated start state, then we've already sent the start,
    // (@@@ we hope), and the TWI statemachine is just waiting for the address byte.
    // We need to remove ourselves from the repeated start state before we enable interrupts,
    // since the ISR is ASYNC, and we could get confused if we hit the ISR before cleaning
    // up. Also, don't enable the START interrupt. There may be one pending from the
    // repeated start that we sent ourselves, and that would really confuse things.
    twi_inRepStart = false;			// remember, we're dealing with an ASYNC ISR
     // write without start bit (already sent)
    I2C_CMD = I2C_WRITE;
  }
  else
    // send start condition and write
    I2C_CMD = I2C_START_WRITE;

  twi_state = TWI_MRX;

  Address_ACK=1;	//indicate we have just sent the addresss
  // wait for read operation to complete
  while(TWI_MRX == twi_state){
    continue;
  }


  if (twi_masterBufferIndex < length)
    length = twi_masterBufferIndex;


  // copy twi buffer to data
  for(i = 0; i < length; ++i)
    data[i] = twi_masterBuffer[i];


  return length;
}



/*
 * Function twi_writeTo
 * Desc     attempts to become twi bus master and write a
 *          series of bytes to a device on the bus
 * Input    address: 7bit i2c device address
 *          data: pointer to byte array
 *          length: number of bytes in array
 *          wait: boolean indicating to wait for write or not
 *          sendStop: boolean indicating whether or not to send a stop at the end
 * Output   0 .. success
 *          1 .. length to long for buffer
 *          2 .. address send, NACK received
 *          3 .. data send, NACK received
 *          4 .. other twi error (lost bus arbitration, bus error, ..)
 */
uint8_t twi_writeTo(uint8_t address, uint8_t* data, uint8_t length, uint8_t wait, uint8_t sendStop)
{
  uint8_t i;

  // ensure data will fit into buffer
  if(TWI_BUFFER_LENGTH < length){
    return 1;
  }

  if(TWI_READY != twi_state){
    twi_error = TW_BUS_ERROR;	//bus error
    return 4;
  }

   // wait until bus is not busy
  while((I2C_STATUS & I2C_STATUS_BUSY) == I2C_STATUS_BUSY ){
    continue;
  }

  twi_sendStop = sendStop;
  // reset error state (0xFF.. no error occurred)
  twi_error = 0xFF;

  // initialize buffer iteration vars
  twi_masterBufferIndex = 0;
  twi_masterBufferLength = length;

  // copy data to twi buffer
  for(i = 0; i < length; ++i){
    twi_masterBuffer[i] = data[i];
  }

  // put address in the TX
  I2C_TX  = address <<1;	//Device address should be only 7 bits the 8th bit determine read or write (here Write as Zero)
  I2C_CTR =  I2C_CTR_EN_INTEN;	//Enable interrupt
  // if we're in a repeated start, then we've already sent the START
  // in the ISR. Don't do it again.
  //
  if (true == twi_inRepStart) { 	//### Check Later###
    // if we're in the repeated start state, then we've already sent the start,
    // (@@@ we hope), and the TWI statemachine is just waiting for the address byte.
    // We need to remove ourselves from the repeated start state before we enable interrupts,
    // since the ISR is ASYNC, and we could get confused if we hit the ISR before cleaning
    // up. Also, don't enable the START interrupt. There may be one pending from the
    // repeated start that we sent outselves, and that would really confuse things.
    twi_inRepStart = false;			// remember, we're dealing with an ASYNC ISR
    // write without start bit (already sent)
    I2C_CMD = I2C_WRITE;
  }
  else
    // send start condition and write
    I2C_CMD = I2C_START_WRITE;

  twi_state = TWI_MTX;	//postponed to here because interrupt is asserted at the end of each transmission which leads to unwanted behaviourm So updating the satate should be late so the previous interrupt doesn't act at all

// wait for write operation to complete
  while(wait && (TWI_MTX == twi_state)){
    continue;
  }

  if (twi_error == 0xFF)
    return 0;	// success
  else if (twi_error == TW_MT_SLA_NACK)
    return 2;	// error: address send, nack received
  else if (twi_error == TW_MT_DATA_NACK)
    return 3;	// error: data send, nack received
  else
    return 4;	// other twi error
}


/*
 * Function twi_stop
 * Desc     relinquishes bus master status
 * Input    none
 * Output   none
 */
void twi_stop(void)
{
  // send stop condition
  I2C_CMD = I2C_STOP;
  I2C_CTR = I2C_CTR_EN;		//disable interrupt

  // update twi state
  twi_state = TWI_READY;
}

/*
 * Function twi_releaseBus
 * Desc     releases bus control
 * Input    none
 * Output   none
 */
void twi_releaseBus(void)
{
  // release bus
  I2C_CTR = I2C_CTR_EN;		//disable interrupt

  // update twi state
  twi_state = TWI_READY;
}

void ISR_I2C(void)
{
  unsigned int  status = I2C_STATUS;
  if ((status & I2C_STATUS_AL) == I2C_STATUS_AL){
	twi_error = TW_MT_ARB_LOST;
        twi_releaseBus();
  }
  else if ((status & I2C_STATUS_RXACK) == 0){ //Ack is active low
	if(twi_state == TWI_MTX){
		if(twi_masterBufferIndex < twi_masterBufferLength){
		// copy data to output register
		I2C_TX = twi_masterBuffer[twi_masterBufferIndex++];
		I2C_CMD = I2C_WRITE;
		}
		else{
			if (twi_sendStop)
			  twi_stop();
			else{
			  twi_inRepStart = true;	// we're gonna send the START
			  // don't enable the interrupt. We'll generate the start, but we
			  // avoid handling the interrupt until we're in the next transaction,
			  // at the point where we would normally issue the start.
			  I2C_CTR = I2C_CTR_EN;		//disable interrupt
			  I2C_CMD = I2C_START;
			  twi_state = TWI_READY;
			}
		}
	}else if(twi_state == TWI_MRX){
		if(Address_ACK)	//ACK was for address
			Address_ACK=0;
		else			//ACK was for data received
			twi_masterBuffer[twi_masterBufferIndex++] = I2C_RX;



		if(twi_masterBufferIndex < twi_masterBufferLength)
			I2C_CMD = I2C_READ;
		else{
			if (twi_sendStop)
			  twi_stop();
			else{
			  twi_inRepStart = true;	// we're gonna send the START
			  // don't enable the interrupt. We'll generate the start, but we
			  // avoid handling the interrupt until we're in the next transaction,
			  // at the point where we would normally issue the start.
			  I2C_CTR = I2C_CTR_EN;		//disable interrupt
			  I2C_CMD = I2C_START;
			  twi_state = TWI_READY;
			}
		}
	}
  }
  else{
  	if(twi_state == TWI_MTX)
	  	if(!twi_masterBufferIndex)
	  		twi_error = TW_MT_SLA_NACK;	//Address was not Acknowledged
	  	else
			twi_error = TW_MT_DATA_NACK;	//Data was not Acknowledged
	twi_stop();
  }

  I2C_CMD |= 0x01;	//clear I2C interrupt pending
  ICP|= (1<<23);		//clear I2C interrupt pending
}
