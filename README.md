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
- delay_microseconds() function because it depends on counting cycles for each instruction


Problems
- software PWM utilizes CPU otherwise would have low frequency 
- when we implement servos later we would face the problem that the 2 timers are already used

Comment
- we can merge PWM timer with millis timer in one timer (frequency should suit some devices like motors)

## UART

Implemented
- Auxiliary classes (String, Print and Stream0
- Hardware porting
Postponed
- Testing (until interrupt works)

## I2C
Not yet

## SPI
Not yet

## General Problems
- WFI (wait for inteerupt) instruction associated with sleep() function causes an illegal instruction exception (in Berkeley's comppiler only)
- ETH compiler don't work on 32 bit machines (could be solved by later commit by Berkeley's compiler)
- FPGA scripts should be ported to ZYBO
- compiler doesn't handle long specifier (I changed all long variables to int unlesss the function is overwritten by another function that uses int)
- Regression tests on Gitlab fail for  testDivRem.test (even for commits that has passed before)

