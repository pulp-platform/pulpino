extern "C" {
#include "twi.h"
}
#include <main.cpp>


volatile int x,y;
unsigned char data='A';

void setup()
{

        twi_init();
        printf("Init \n");
	y= twi_writeTo(127, &data, 1, 1, 1);
        printf("Y= %d",y);
}

void loop()
{
x++;
}



