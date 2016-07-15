
#include <main.cpp>


void setup(){
pinMode(0,OUTPUT);
pinMode(1,OUTPUT);
pinMode(2,OUTPUT);

}


void loop(){
digitalWrite(0,HIGH);
analogWrite(1,50);
analogWrite(2,200);
delay(20);
analogWrite(0,125);
analogWrite(1,300);
digitalWrite(2,LOW);
delay(20);
}

