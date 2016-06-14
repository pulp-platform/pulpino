
#include <main.cpp>

int x;

void setup()
{
  pinMode(1,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,INPUT);
  
}

void loop()
{
  x=digitalRead(4);
  digitalWrite(1,HIGH);
  digitalWrite(2,LOW);
  digitalWrite(3,x);
}



