
#include <main.cpp>

const byte ledPin = 13;
const byte interruptPin1 = 1;
const byte interruptPin2 = 2;
volatile byte state = LOW;

void blink() {
  state = !state;
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin1, INPUT);
  pinMode(interruptPin2, INPUT);
  attachInterrupt(interruptPin1, blink, RISING);
  attachInterrupt(interruptPin2, blink, RISING);
}

void loop() {
  digitalWrite(ledPin, state);
}




