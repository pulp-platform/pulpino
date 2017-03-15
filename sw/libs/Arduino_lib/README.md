# General Overview

The currently supported Arduino libraries are in general; the core libraries and the priepheral libraries like SPI and I2C with details in the following sections.

These libraries were tested and their functionality was verified through simulation.
The device drivers for Servo, SD, LCD .. etc are not yet verified using FPGA, they may require some modifications on the original Arduino libraries. This should be done soon.

## Adding Arduino sketch to PULPino

You can add Arduino sketches, compile and test them using the same procedures described in `sw` subfolder.
You only need to include main.cpp at the beginning of the program:

	#include "main.cpp"



# Status


## Core Libraries in General
Postponed:
- PulseIn function should have a part of it in assembly to know the determined cycles for that part (curently it's implemented totally in C)
- Tone function as it uses timers and we only have two already used timers each with one channel and no PWM, and later when implementing the servo will need to handle it again. So better to handle the timers all together one time later.
- AnalogRead function is not implemented as current version of PULPino doesn't support Analog to Digital converter (ADC).

## GPIO
Completed for simulation and FPGA targets. Only pad mux should be considered for Imperio version.

## Interrupt
Completed but need to handle function calls if we ar going to use interrupt attribute instead of current crt0 approach.

## Timer
Postponed:
- delay_microseconds() function should be hardcoded depending on the frequency used to minimize error (currently a generic function is developed instead)

Problems:
- software PWM utilizes CPU if high frequency required.

Comment:
- we can merge PWM timer with millis timer in one timer, this would help the problems(frequency should suit some devices like motors).

## UART

Completed using busy waiting approach, but the best approach is to modify it using interrupt based approach.


## Separated Libraries in General
Problems:
- softwareSerial couldn't be implemented as it needs GPIO interrupts on toggle (any change rising or falling edge) which is not vailable on current version of PULPino.

## I2C
Completed except for I2C slave functions as the current PULPino version supports I2C as a master only.
Problems

## SPI
Completed.
