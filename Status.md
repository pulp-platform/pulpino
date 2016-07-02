
# Status

## GPIO
	Done
- implementation 
- test
	postponed
- pad mux
- input_pullup (for imperio)
- interrupt disable (SREG issue in AVR)
	Problems
- None

## Interrupt
	Done
- implementation
	Postponed
- minimizing function calls inside interrupt functions
- using interrupt atribute instead of crt0 approach
	problems
- test doesnt respond even for already implemented tests (seems problem in my machine)
- ETH's compiler doesn't accept function calls in interupt attributed functions

## Timer
	Done
- implementation
- software PWM
	Postponed
- delay_microseconds() function because it depends on counting cycles for each instruction
- test untill interrupt test passes
	Problems
- software PWM utilizes CPU otherwise would have low frequency
- when we implement servos later we would face the problem that the 2 timers are already used

## UART
	Implemented
- Auxiliary classes (String, Print and Stream0
	Ongoing
- Harware porting

## I2C
Not yet

## SPI
Not yet

## General Problems
- WFI (wait for inteerupt) instruction associated with sleep() function causes an illegal instruction exception
- ETH compiler don't work on my machine (32 bit machines compatibility issue)
- FPGA scripts should be ported to ZYBO

