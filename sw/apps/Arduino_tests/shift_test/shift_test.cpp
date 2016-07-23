
#include <main.cpp>

volatile uint8_t x;

void setup()
{
  pinMode(0,OUTPUT);
  pinMode(1,OUTPUT);
  pinMode(2,INPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);

shiftOut(0, 1, 0, 5);
digitalWrite(4,HIGH);
x=shiftIn(2, 3, 0);
printf("Read %u \n",x); 	
	
  
}

void loop()
{
x++;
}



