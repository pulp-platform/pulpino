
#include <main.cpp>

// self test variables
int irqPinToggles = 0;

const byte ledPin = 13;
const byte interruptPin1 = 1;
const byte interruptPin2 = 2;
byte state = LOW;

void blink() {
  state = !state;

  irqPinToggles++;
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin1, INPUT);
  pinMode(interruptPin2, INPUT);
  attachInterrupt(interruptPin1, blink, RISING);
  attachInterrupt(interruptPin2, blink, RISING);
  pinMode(0,OUTPUT);	//used to intiate test bench stimulus (for simulation only)
  digitalWrite(0,HIGH);  
}

void loop() {
  digitalWrite(ledPin, state);

  // check for test condition
  if (irqPinToggles == 3)
    exit(0);	//last signal won't be shown in modelsim as simulation will end
}
