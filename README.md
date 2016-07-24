# Status

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

problems
- when two GPIO interrupts are attached, assertion errors occur and the GPIO_ISR is not entered when any event happens

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


## Core Libraries

Postponed
- PulseIn function should have a part of it in assembly to know the determined cycles for that part (curently it's implemented totally in C)
- Tone function as it uses timers and we on;y have two already used timers each with one channel and no PWM, and later when implementing the servo will need to handle it again. So better to handle the timers all together one time later.

## I2C
Not yet

## SPI
Not yet

## General Problems
- WFI (wait for inteerupt) instruction associated with sleep() function causes an illegal instruction exception (in Berkeley's comppiler only)
- ETH compiler don't work on 32 bit machines (could be solved by later commit by Berkeley's compiler)
- FPGA scripts should be ported to ZYBO
- Regression tests on Gitlab fail for  testDivRem.test (even for commits that has passed before)

