#include "utils.h"
#include "string_lib.h"
#include "bar.h"

#include "bf_skey.h"
#include "blowfish.h"
#include "jrand.c"

__attribute__ ((section(".heapsram"))) BF_KEY key;
__attribute__ ((section(".heapsram"))) unsigned char ukey[8];
__attribute__ ((section(".heapsram"))) unsigned char indata[40];
__attribute__ ((section(".heapsram"))) unsigned char outdata[40];
__attribute__ ((section(".heapsram"))) unsigned char ivec[8];
__attribute__ ((section(".heapsram"))) BF_KEY bf_init;


char *ckey = "ABCDEF1234567890";

const unsigned char check_outdata[40] = {
  -11, 25, 13, -69, -45, -32, 31, 46,
  -77, -34, 57, -26, 1, -125, -65, 119,
  67, -82, -23, -42, -47, 51, 15, 71,
  83, 30, 89, -58, 33, 67, -97, 87,
  -61, -114, -87, -42, -111, -45, 15, 71
};

void compute_bf();

int main()
{
  int error = 0;

  // initialize jrand
  next = 1;
   
 

  printf("Start BF test %d\n",0,0,0);

  int i,f,k;
  load_key(&bf_init);
  for (f=0;f<1;f++){
  
    // initialize ivec to 0
    for (k=0;k<8;k++){
ivec[k]=0;
    }     
    compute_bf();
  }
   
  // verify that we have the correct result.
  for (i = 0; i < 40; i++) {
    if (outdata[i] != check_outdata[i]) {
printf("Wrong result:\n Index: %d Result: %d does not match expected result: %d\n",i,outdata[i],check_outdata[i],0);
error+=1;
    }
  }

  if (error == 0){
    printf("Ok!!\n",0,0,0,0);
  }
  else {
    printf("Not OK! %d errors occured!\n",error,0,0,0);
  }

  /* int *DEFAULT_RESULT; */
  /* if (error == 0) { */
  /*   DEFAULT_RESULT = (int*)0x10003ffc; */
  /*   *(DEFAULT_RESULT) = 1; */
  /* } */
  /* else { */
  /*   DEFAULT_RESULT = (int*)0x10003ffc; */
  /*   *(DEFAULT_RESULT) = error; */
  /* } */



 
  eoc(0);
}


void compute_bf(){

  int num;
  int by=0,i=0,k=0,f=0;
  int encordec=-1;
  char *cp,ch;
  int n, n2;

     
  for(n = 0; n < REPEAT_FACTOR>>11; ++n){
    encordec = 1;
    num=0;
       
    /* Read the key */
    cp = ckey;
    while(i < 64 && *cp)    /* the maximum key length is 32 bytes and   */
      {                       /* hence at most 64 hexadecimal digits      */
	ch = *cp++;            /* process a hexadecimal digit  */
	if(ch >= '0' && ch <= '9')
	  by = (by << 4) + ch - '0';
	else if(ch >= 'A' && ch <= 'F')
	  by = (by << 4) + ch - 'A' + 10;
	else                            /* error if not hexadecimal     */
	  {
	    //printf("key must be in hexadecimal notation\n",0,0,0,0);
	    //exit(-1);
	  }
	   
	/* store a key byte for each pair of hexadecimal digits         */
	if(i++ & 1)
	  ukey[i / 2 - 1] = by & 0xff;
      }
       
    BF_set_key(&key,8,ukey,&bf_init);
       
    if(*cp)
      {
	//printf("Bad key value.\n",0,0,0,0);
	//exit(-1);
      }
       
    i=0;
    for(n2 = 0; n2 < 256; ++n2)
      {
	while(i<40)
	  indata[i++]=jrand();
	BF_cfb64_encrypt(indata,outdata,i,&key,ivec,&num,encordec);
	encordec = 1-encordec;
	BF_cfb64_encrypt(outdata,indata,i,&key,ivec,&num,encordec);
	   
	i=0;
      }
  }
}

