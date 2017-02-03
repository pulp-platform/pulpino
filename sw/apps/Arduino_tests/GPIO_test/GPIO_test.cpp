
#include <main.cpp>

int x;

void setup()
{
  pinMode(1,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,INPUT);
  pinMode(7,INPUT);	//used to terminate test bench (for simulation only)
  pinMode(0,OUTPUT);	//used to intiate test bench stimulus (for simulation only)
  digitalWrite(0,HIGH);  
}

void loop()
{
  x=digitalRead(4);
  digitalWrite(1,HIGH);
  digitalWrite(2,LOW);
  digitalWrite(3,x);
  if (digitalRead(7))
	exit(0);	//to end simulation properly
}



