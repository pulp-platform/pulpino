/*
  Arduino.h - Main include file for the Arduino SDK
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

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
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA3
  
  Modified 14 June 2016 by Mahmoud Elmohr       (Ported to RISC-V PULPino)
*/


#ifndef Arduino_h
#define Arduino_h

#define F_CPU 25000000U	// Put here temporarily but later IDE should put it

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdint.h>	//Added for uint_t

#include "int.h"
#include "binary.h"	

#ifdef __cplusplus
extern "C"{
#endif

void yield(void);	// an empty function unless overwritten for multi threading

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2	// ###check later###

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1


//#define CHANGE 1 	//there is no change in PULPino, there's high instead
// 0 is low 1 is high (already defined)
#define RISING 2	//modified to be 2 instead of 3 as in AVR
#define FALLING 3	//modified to be 3 instead of 2 as in AVR


// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define interrupts() int_enable()
#define noInterrupts() int_disable()


#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))	
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))


#ifndef _NOP
#define _NOP() __asm__ volatile ("nop"); 
#endif


typedef unsigned int word;

#define bit(b) (1UL << (b))

typedef bool boolean;
typedef uint8_t byte;

void pinMode(uint8_t, uint8_t);
void digitalWrite(uint8_t, uint8_t);
int digitalRead(uint8_t);

void init(void);	
unsigned int millis(void);	
unsigned int micros(void);	
void delay(unsigned int);	
void delayMicroseconds(unsigned int us);	

void analogWrite(uint8_t, int);	

unsigned int pulseIn(uint8_t pin, uint8_t state, unsigned int timeout);	
unsigned int pulseInLong(uint8_t pin, uint8_t state, unsigned int timeout);	

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);	
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);	

void initVariant(void);	

int atexit(void (*func)()) __attribute__((weak));

/////////////////Analog Read/////////////////
//current version of PULpino doesn't contain ADC
/* 
int analogRead(uint8_t);	// ###check later###
void analogReference(uint8_t mode);	// ###check later###
*/
///////////////////////End///////////////////////

void attachInterrupt(uint8_t, void (*)(void), int mode);
void detachInterrupt(uint8_t);

void setup(void);
void loop(void);

#define digitalPinToBitMask(P) ( 1<< (P) )

static inline int BitMaskToDigitalPin(unsigned int mask){
int pin =0;
while(mask!=1){
	mask>>=1;
	pin++;
	}
return pin;
}


#define NOT_A_PIN 0
#define NOT_AN_INTERRUPT -1
#
#define NOT_ON_TIMER 0
#define TIMERA 1
#define TIMERB 2

#ifdef __cplusplus
} // extern "C"
#endif


#ifdef __cplusplus

#include "WString.h"
#include "HardwareSerial.h"
#include "WCharacter.h"

uint16_t makeWord(uint16_t w);	//word here is 16 bit for compatibility with arduino
uint16_t makeWord(byte h, byte l);	
#define word(...) makeWord(__VA_ARGS__)	//###check later###


// WMath prototypes
int random(int);
int random(int, int);
int randomSeed(unsigned int);
int map(int, int, int, int, int);

unsigned int pulseIn(uint8_t pin, uint8_t state, unsigned int timeout = 1000000U);	
unsigned int pulseInLong(uint8_t pin, uint8_t state, unsigned int timeout = 1000000U);	


//////////////Tone///////////////
// tone is not implemented yet
/*
void tone(uint8_t _pin, unsigned int frequency, unsigned int duration = 0);	//###check later###
void noTone(uint8_t _pin);	//###check later###
*/
////////////////////////End///////////////////////
#endif

#include "pins_arduino.h"

#endif
