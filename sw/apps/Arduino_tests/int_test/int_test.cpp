
#include <main.cpp>

const byte ledPin = 13;
const byte interruptPin = 2;
volatile byte state = LOW;

void blink() {
  state = !state;
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT);
  attachInterrupt(2, blink, RISING);
}

void loop() {
  digitalWrite(ledPin, state);
}




