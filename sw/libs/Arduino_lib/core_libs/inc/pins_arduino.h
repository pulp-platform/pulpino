/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  Modified 14 June 2016 by Mahmoud Elmohr       (Ported to RISC-V PULPino)
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h


# if defined(Imperio)
#define NUM_DIGITAL_PINS            19	//GPIO for Imperio
#elif defined(Zed)	
#define NUM_DIGITAL_PINS            30	//GPIO for ZedBoard
#else
#define NUM_DIGITAL_PINS            32	//GPIO for Simulation
#endif




//Peripheral pins numbers
static const uint8_t SS   = 16;	
static const uint8_t MOSI = 15;	
static const uint8_t MISO = 14;	
static const uint8_t SCK  = 19;	//it doesn't share pad with GPIO

static const uint8_t SDA = 17;	
static const uint8_t SCL = 18;	

//there are other pins in PULPino bit not used in Arduino, we may add new features later


#define LED_BUILTIN 13

#define digitalPinToInterrupt(p)   ((p) < NUM_DIGITAL_PINS) ? (p) : (NOT_AN_INTERRUPT)	//in PULPino all GPIOs have interrupt


// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_MONITOR   Serial
#define SERIAL_PORT_HARDWARE  Serial

#endif
