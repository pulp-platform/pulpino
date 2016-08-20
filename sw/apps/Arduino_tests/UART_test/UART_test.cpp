//Output shoud be :
// 0  intialized in the rx 
// 101 asserted to tx by the test bench
// 7 intialized for the variable (no thing to read)


#include "main.cpp"

byte thisByte[3] = {5, 6, 7};
int x;

void setup() {
  
  pinMode(0,OUTPUT);
  Serial.begin(781250);
  
  digitalWrite(0,HIGH);

  delayMicroseconds(100);

  for (x=0;x<3;x++){
	  if (Serial.available()){
	  	thisByte[x]=Serial.read();
	  } 
  }

  for (x=0;x<3;x++){

  	  Serial.write(thisByte[x]);

	  Serial.print(", dec: ");
	  // prints value as string as an ASCII-encoded decimal (base 10).
	  // Decimal is the  default format for Serial.print() and Serial.println(),
	  // so no modifier is needed:
	  Serial.print(thisByte[x]);
	  // But you can declare the modifier for decimal if you want to.
	  //this also works if you uncomment it:

	  // Serial.print(thisByte, DEC);


	  Serial.print(", hex: ");
	  // prints value as string in hexadecimal (base 16):
	  Serial.print(thisByte[x], HEX);

	  Serial.print(", oct: ");
	  // prints value as string in octal (base 8);
	  Serial.print(thisByte[x], OCT);

	  Serial.print(", bin: ");
	  // prints value as string in binary (base 2)
	  // also prints ending line break:
	  Serial.println(thisByte[x], BIN);
  }
	 
}

void loop(){
  digitalWrite(0,HIGH);

  delayMicroseconds(100);

  for (x=0;x<3;x++){
	  if (Serial.available()){
	  	thisByte[x]=Serial.read();
	  } 
  }

  for (x=0;x<3;x++){

  	  Serial.write(thisByte[x]);

	  Serial.print(", dec: ");
	  // prints value as string as an ASCII-encoded decimal (base 10).
	  // Decimal is the  default format for Serial.print() and Serial.println(),
	  // so no modifier is needed:
	  Serial.print(thisByte[x]);
	  // But you can declare the modifier for decimal if you want to.
	  //this also works if you uncomment it:

	  // Serial.print(thisByte, DEC);


	  Serial.print(", hex: ");
	  // prints value as string in hexadecimal (base 16):
	  Serial.print(thisByte[x], HEX);

	  Serial.print(", oct: ");
	  // prints value as string in octal (base 8);
	  Serial.print(thisByte[x], OCT);

	  Serial.print(", bin: ");
	  // prints value as string in binary (base 2)
	  // also prints ending line break:
	  Serial.println(thisByte[x], BIN);
  }
	 
   exit(0);	//to end simulation properly
}
