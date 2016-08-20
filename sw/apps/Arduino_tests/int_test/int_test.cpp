
#include <main.cpp>

const byte ledPin = 13;
const byte interruptPin1 = 1;
const byte interruptPin2 = 2;
byte state = LOW;

void blink() {
  state = !state;
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin1, INPUT);
  pinMode(interruptPin2, INPUT);
  attachInterrupt(interruptPin1, blink, RISING);
  attachInterrupt(interruptPin2, blink, RISING);
  pinMode(7,INPUT);	//used to terminate test bench (for simulation only)
  pinMode(0,OUTPUT);	//used to intiate test bench stimulus (for simulation only)
  digitalWrite(0,HIGH);  
}

void loop() {
  digitalWrite(ledPin, state);
  if (digitalRead(7))
	exit(0);	//to end simulation properly
}




