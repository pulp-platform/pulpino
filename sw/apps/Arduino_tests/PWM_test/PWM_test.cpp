
#include <main.cpp>


void setup(){
pinMode(0,OUTPUT);
pinMode(1,OUTPUT);
pinMode(2,OUTPUT);
analogWrite(1,50);
analogWrite(2,200);
}


void loop(){
digitalWrite(0,HIGH);
}

