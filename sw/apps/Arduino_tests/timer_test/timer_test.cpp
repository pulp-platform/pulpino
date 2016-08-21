
#include <main.cpp>



void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(3);             
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(3);
            
  exit(0);	//to end simulation properly
}

