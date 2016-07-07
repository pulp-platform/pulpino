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
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef Arduino_h
#define Arduino_h

#define F_CPU 5000000U	// Put here temporarily but later IDE should put it

// ###check later###
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include<stdint.h>	//Added for uint_t

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

//////////////////////// Modification///////////////////////
//#define CHANGE 1 	//there is no change in PULPino, there's high instead
// 0 is low 1 is high (already defined)
#define RISING 2	//modified to be 2 instead of 3 as in AVR
#define FALLING 3	//modified to be 3 instead of 2 as in AVR
/////////////////////////////End////////////////////////////


//////////////////////// Supression///////////////////////
//these are AVR definitions for AVR varient ICs, ###check later###
/*
#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
#define DEFAULT 0
#define EXTERNAL 1
#define INTERNAL 2
#else  
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
#define INTERNAL1V1 2
#define INTERNAL2V56 3
#else
#define INTERNAL 3
#endif
#define DEFAULT 1
#define EXTERNAL 0
#endif
*/
/////////////////////////////End////////////////////////////

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

//////////////////////// Supression///////////////////////
//these are functions provided by AVR libraries that needs to be built for PULPino, ###check later###
/*
#define interrupts() sei()
#define noInterrupts() cli()
*/
////////////////////////End///////////////////////


#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 24))	//changed to be 24 insteadof 8, we may need the middle bytes too ###check later###
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))


//////////////////////// Supression///////////////////////
//Not sure if we have NOP pseudocod in the compiler or wil need to write the shift zero, ###check later###
/*
// avr-libc defines _NOP() since 1.6.2
#ifndef _NOP
#define _NOP() do { __asm__ volatile ("nop"); } while (0)
#endif
*/
////////////////////////End///////////////////////


typedef unsigned int word;

#define bit(b) (1UL << (b))

typedef bool boolean;
typedef uint8_t byte;

void pinMode(uint8_t, uint8_t);
void digitalWrite(uint8_t, uint8_t);
int digitalRead(uint8_t);

void init(void);	
unsigned long millis(void);	
unsigned long micros(void);	
void delay(unsigned long);	
void delayMicroseconds(unsigned int us);	

void analogWrite(uint8_t, int);	

//////////////////////// Supression///////////////////////
/*
void initVariant(void);	// ###check later###

int atexit(void (*func)()) __attribute__((weak));	// ###check later###


int analogRead(uint8_t);	// ###check later###
void analogReference(uint8_t mode);	// ###check later###

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout);	// ###check later###
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout);	// ###check later###

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);	// ###check later###
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);	// ###check later###

*/
///////////////////////End///////////////////////

void attachInterrupt(uint8_t, void (*)(void), int mode);
void detachInterrupt(uint8_t);

void setup(void);
void loop(void);


//extern const uint8_t PROGMEM digital_pin_to_timer_PGM[];	// ###check later###
//#define digitalPinToTimer(P) ( pgm_read_byte( digital_pin_to_timer_PGM + (P) ) )	// ###check later###
//#define analogInPinToBit(P) (P)	// ###check later###

#define digitalPinToBitMask(P) ( 1<< (P) )

static inline int BitMaskToDigitalPin(uint32_t mask){
int pin =0;
while(mask!=0){
	mask>>=1;
	pin++;
	}
return pin;
}


#define NOT_A_PIN 0

#define NOT_AN_INTERRUPT -1

//////////////////////// Supression///////////////////////
/*
// ###check later###
#define NOT_ON_TIMER 0
#define TIMER0A 1
#define TIMER0B 2
#define TIMER1A 3
#define TIMER1B 4
#define TIMER1C 5
#define TIMER2  6
#define TIMER2A 7
#define TIMER2B 8

#define TIMER3A 9
#define TIMER3B 10S
#define TIMER3C 11
#define TIMER4A 12
#define TIMER4B 13
#define TIMER4C 14
#define TIMER4D 15
#define TIMER5A 16
#define TIMER5B 17
#define TIMER5C 18
*/
///////////////////////End///////////////////////

#ifdef __cplusplus
} // extern "C"
#endif


#ifdef __cplusplus

#include "WString.h"
#include "HardwareSerial.h"
#include "WCharacter.h"

uint16_t makeWord(uint16_t w);	//word here is 16 bit for compatibility with arduino
uint16_t makeWord(byte h, byte l);	//###check later###
#define word(...) makeWord(__VA_ARGS__)	//###check later###


// WMath prototypes
int random(int);
int random(int, int);
int randomSeed(unsigned int);
int map(int, int, int, int, int);

////////////////////////Supression///////////////////////
//Don't need to include other libraries now, ###check later###
/*
#include "USBAPI.h"
*/
////////////////////////End///////////////////////

////////////////////////Supression///////////////////////
/*
// ###check later###
#if defined(HAVE_HWSERIAL0) && defined(HAVE_CDCSERIAL)
#error "Targets with both UART0 and CDC serial not supported"
#endif

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);	//###check later###
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);	//###check later###

void tone(uint8_t _pin, unsigned int frequency, unsigned long duration = 0);	//###check later###
void noTone(uint8_t _pin);	//###check later###

*/
////////////////////////End///////////////////////
#endif

#include "pins_arduino.h"

#endif
