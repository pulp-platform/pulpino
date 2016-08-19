# Status



## Core Libraries in General
Postponed
- PulseIn function should have a part of it in assembly to know the determined cycles for that part (curently it's implemented totally in C)
- Tone function as it uses timers and we only have two already used timers each with one channel and no PWM, and later when implementing the servo will need to handle it again. So better to handle the timers all together one time later.

## GPIO
Done
- implementation 
- test

postponed
- pad mux
- input_pullup (for imperio)

Problems
- None

## Interrupt
Done
- implementation
- test

Postponed
- minimizing function calls inside interrupt functions
- using interrupt atribute instead of crt0 approach

## Timer
Done
- implementation
- software PWM
- test

Postponed
- delay_microseconds() function because it should be hardcoded depending on the frequency used (I wrote one generic function but need to be overriden later)


Problems
- software PWM utilizes CPU otherwise would have low frequency 
- software PWM is not accurate as the other timer interrupts it sometimes, and if mutual exclusion used sometimes the other timer doesn't work
- when we implement servos later we would face the problem that the 2 timers are already used

Comment
- we can merge PWM timer with millis timer in one timer, this would help the problems(frequency should suit some devices like motors)

## UART

Implemented
- Auxiliary classes (String, Print and Stream)
- Hardware porting (using busy waiting approach not interrrupt)
- test

Postponed
- modify th elibraries to be interrupt based



## Separated Libraries in General
Problems
- softwareSerial couldn't be implemented as it needs GPIO interrupts on toggle (any change rising or falling edge) which is not vailable on current version of PULPino

## I2C
Implemented
- twi Library (Transmitter & Reciever)
- Wire Library

Problems
- the current PULPino version isn't design tp be have as a slave at all, So all the slave functions are removed from the modified library

## SPI
Implemented
- SPI Master library of Arduino (Arduino doesn't have official lib for SPI slave)



## General Problems
- WFI (wait for interrupt) instruction associated with sleep() function causes an illegal instruction exception (in Berkeley's comppiler only)
- ETH compiler don't work on 32 bit machines (could be solved by later commit by Berkeley's compiler)
- ETH compiler doesn't allow function calls for interrupts if used interrupt attribute 


