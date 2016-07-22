
#include <main.cpp>

unsigned int x;

void setup()
{
  pinMode(4,INPUT);
 
  x= pulseIn(4,1);
  printf("%u \n",x);
 
  x= pulseInLong(4,1);
  printf("%u \n",x);
  
}

void loop()
{

}



