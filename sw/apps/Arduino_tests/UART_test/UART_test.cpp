//Output shoud be : 101 decimal

#include "main.cpp"

byte thisByte=0;


void setup() {
  
  pinMode(0,OUTPUT);
  Serial.begin(781250);
	 
}

void loop(){
  digitalWrite(0,HIGH);

  delayMicroseconds(100);

  if (Serial.available())
  thisByte=Serial.read();
	   
  

  Serial.write(thisByte);
  Serial.print(", dec: ");
  // prints value as string as an ASCII-encoded decimal (base 10).
  // Decimal is the  default format for Serial.print() and Serial.println(),
  // so no modifier is needed:
  Serial.print(thisByte);
  // But you can declare the modifier for decimal if you want to.
  //this also works if you uncomment it:
  // Serial.print(thisByte, DEC);

  Serial.print(", hex: ");
  // prints value as string in hexadecimal (base 16):
  Serial.print(thisByte, HEX);

  Serial.print(", oct: ");
  // prints value as string in octal (base 8);
  Serial.print(thisByte, OCT);

  Serial.print(", bin: ");
  // prints value as string in binary (base 2)
  // also prints ending line break:
  Serial.println(thisByte, BIN);
  
  delay(1); 
	 
  exit(0);	//to end simulation properly
}
