// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include <stdio.h>

#define pixel short int

#include "background_80_60.h"
#include "test_omino_80_60.h"

#define PICTURE_HORIZONTAL_SIZE 10
#define PICTURE_VERTICAL_SIZE 10
#define PICTURE_HORIZONTAL_OFFSET 15
#define KERNEL_SIZE 3

pixel sobel1[9] = {1,2,1,0,0,0,-1,-2,-1};
pixel sobel2[9] = {1,0,-1,2,0,-2,1,0,-1};

void val_abs(pixel *image_in)
{
  int i,j=0;
  int tmp,trush;
  
  for(i=0;i<PICTURE_VERTICAL_SIZE;i++){
    for(j=0;j<PICTURE_HORIZONTAL_SIZE;j++){
      
      if(image_in[i*PICTURE_HORIZONTAL_SIZE+j]<0){
	image_in[i*PICTURE_HORIZONTAL_SIZE+j]=-image_in[i*PICTURE_HORIZONTAL_SIZE+j];
      }
    }
  }

}//val_abs

void sum_image(pixel *image_in1, pixel *image_in2){
  int i,j=0;
  int tmp,trush;
  
  for(i=0;i<PICTURE_VERTICAL_SIZE;i++){
    for(j=0;j<PICTURE_HORIZONTAL_SIZE;j++){
      image_in1[i*PICTURE_HORIZONTAL_SIZE+j]=
	(image_in1[i*PICTURE_HORIZONTAL_SIZE+j])+(image_in2[i*PICTURE_HORIZONTAL_SIZE+j]);
    }
  }
  
}

void sub_image(pixel *image_in1, pixel *image_in2){
  int i,j=0;
  short int tmp;
  
  for(i=0;i<PICTURE_VERTICAL_SIZE;i++){
    for(j=0;j<PICTURE_HORIZONTAL_SIZE;j++){
      tmp = image_in1[i*PICTURE_HORIZONTAL_SIZE+j]-image_in2[i*PICTURE_HORIZONTAL_SIZE+j];
      if (tmp < 0)
	image_in1[i*PICTURE_HORIZONTAL_SIZE+j] = -tmp;
      else
	image_in1[i*PICTURE_HORIZONTAL_SIZE+j] = tmp;
    }
  }
}

pixel  max_image(pixel *image_in){
 
  int i,j=0;
  int trush;
  int tmp;
  pixel  max_pix;
   
  for(i=0;i<PICTURE_VERTICAL_SIZE;i++){
    for(j=0;j<PICTURE_HORIZONTAL_SIZE;j++){     
      if(image_in[i*PICTURE_HORIZONTAL_SIZE+j]>max_pix){
	max_pix=image_in[i*PICTURE_HORIZONTAL_SIZE+j];
      }
    }//for
  }//for
   
  return max_pix;
}


pixel  min_image(pixel *image_in){
  
  int i,j=0;
  int trush;
  int tmp;
  pixel  min_pix;
  
  for(i=0;i<PICTURE_VERTICAL_SIZE;i++){
    for(j=0;j<PICTURE_HORIZONTAL_SIZE;j++){     
      
      if(image_in[i*PICTURE_HORIZONTAL_SIZE+j]>min_pix){
	min_pix=image_in[i*PICTURE_HORIZONTAL_SIZE+j];
      }
    }
  }//for
  return min_pix;
}//min_image


void binarisation(pixel *image_in, pixel thresh, pixel val_max, pixel val_min)
{
  int i=0;
  int k,j=0;
 
  for(i=0;i<PICTURE_VERTICAL_SIZE;i++) {
    for(j=0;j<PICTURE_HORIZONTAL_SIZE;j++) {
      if(image_in[i*PICTURE_HORIZONTAL_SIZE+j]<thresh)
	image_in[i*PICTURE_HORIZONTAL_SIZE+j]=val_min;
      else
	image_in[i*PICTURE_HORIZONTAL_SIZE+j]=val_max;
    }
  }
}//binarisation

void erosion( pixel *image_in, int ker_size, pixel *image_out){
  
  int x,y,i,j,k;
  int tmp_a,tmp_b,tmp_c;
  int var;
  pixel min = 255;
  
  for(x=1;x<PICTURE_VERTICAL_SIZE-1;x++) {
    for(y=1;y<PICTURE_HORIZONTAL_SIZE-1;y++) {
      min=255;
      for(i=0;i<ker_size;i++) {
	for(j=0;j<ker_size;j++) {
	  if (image_in[(x-1)*PICTURE_HORIZONTAL_SIZE + (y-1) + (i*PICTURE_HORIZONTAL_SIZE+j)] < min ){
	    min=image_in[(x-1)*PICTURE_HORIZONTAL_SIZE + (y-1) + (i*PICTURE_HORIZONTAL_SIZE+j)];
	  }
	}
      }
      image_out[x*PICTURE_HORIZONTAL_SIZE+y] = min;
    }
  }
  
  /**************************/
  /*     Boundary Copy      */      
  /**************************/
  
  for(k=0;k<PICTURE_HORIZONTAL_SIZE;k++){
    
    image_out[k]=image_in[k];
    image_out[(PICTURE_VERTICAL_SIZE-1)*PICTURE_HORIZONTAL_SIZE+k]=image_in[(PICTURE_VERTICAL_SIZE-1)*PICTURE_HORIZONTAL_SIZE+k];
  }
  for(k=0;k<PICTURE_VERTICAL_SIZE;k++){
    image_out[k*PICTURE_HORIZONTAL_SIZE]=image_in[k*PICTURE_HORIZONTAL_SIZE];
    image_out[k*PICTURE_HORIZONTAL_SIZE+PICTURE_HORIZONTAL_SIZE-1]=image_in[k*PICTURE_HORIZONTAL_SIZE+PICTURE_HORIZONTAL_SIZE-1];
  }
}//erosion


void dilatation( pixel *image_in, int ker_size, pixel *image_out ){
  
  int x,y,i,j, k;
  unsigned int tmp,trush;
  unsigned int *tmpA, *tmpB, *tmpC, *tmp_tmp;
  unsigned int *outA, *outB, *outC, *out_tmp;
  pixel max=0;
  
  for(x=1;x<PICTURE_VERTICAL_SIZE-1;x++) {
    for(y=1;y<PICTURE_HORIZONTAL_SIZE-1;y++) {  
      max=0;
      for(i=0;i<ker_size;i++) {
	for(j=0;j<ker_size;j++) {
	  
	  if (image_in[(x-1)*PICTURE_HORIZONTAL_SIZE + (y-1) + (i*PICTURE_HORIZONTAL_SIZE+j)] > max ){
	    max=image_in[(x-1)*PICTURE_HORIZONTAL_SIZE + (y-1) + (i*PICTURE_HORIZONTAL_SIZE+j)];
	  }
	}
      }
      image_out[x*PICTURE_HORIZONTAL_SIZE+y] = max;	  
    } 
  }//for
  
  /**************************/
  /*     Boundary Copy      */      
  /**************************/
  
  for(k=0;k<PICTURE_HORIZONTAL_SIZE;k++){

    image_out[k]=image_in[k];
    image_out[(PICTURE_VERTICAL_SIZE-1)*PICTURE_HORIZONTAL_SIZE+k]=image_in[(PICTURE_VERTICAL_SIZE-1)*PICTURE_HORIZONTAL_SIZE+k];
  }
  for(k=0;k<PICTURE_VERTICAL_SIZE;k++){
    image_out[k*PICTURE_HORIZONTAL_SIZE]=image_in[k*PICTURE_HORIZONTAL_SIZE];
    image_out[k*PICTURE_HORIZONTAL_SIZE+PICTURE_HORIZONTAL_SIZE-1]=image_in[k*PICTURE_HORIZONTAL_SIZE+PICTURE_HORIZONTAL_SIZE-1];
  }

}//dilatation

void convolution_rect( pixel *image_in, int ker_size, pixel *kernel, pixel *image_out){
  
  int x,y,k,i,j;
  
  for(x=1;x<PICTURE_VERTICAL_SIZE-1;x++){
    for(y=1;y<PICTURE_HORIZONTAL_SIZE-1;y++){
      image_out[x*PICTURE_HORIZONTAL_SIZE+y]=0;
      for(i=0;i<ker_size;i++){
	for(j=0;j<ker_size;j++){  
	  image_out[x*PICTURE_HORIZONTAL_SIZE+y]+=
	    image_in[(x-1)*PICTURE_HORIZONTAL_SIZE + (y-1) + i*PICTURE_HORIZONTAL_SIZE+j]*kernel[i*ker_size+j];
	}  
      }
      // image_out[x*PICTURE_HORIZONTAL_SIZE+y]-= image_in[x*PICTURE_HORIZONTAL_SIZE+y];
    }
  }//for

  for(k=0;k<PICTURE_HORIZONTAL_SIZE;k++){
    image_out[k]=image_in[k];
    image_out[(PICTURE_VERTICAL_SIZE-1)*PICTURE_HORIZONTAL_SIZE+k]=image_in[(PICTURE_VERTICAL_SIZE-1)*PICTURE_HORIZONTAL_SIZE+k];
  }//for
  for(k=0;k<PICTURE_VERTICAL_SIZE;k++){
    image_out[k*PICTURE_HORIZONTAL_SIZE]=image_in[k*PICTURE_HORIZONTAL_SIZE];
    image_out[k*PICTURE_HORIZONTAL_SIZE+PICTURE_HORIZONTAL_SIZE-1]=image_in[k*PICTURE_HORIZONTAL_SIZE+PICTURE_HORIZONTAL_SIZE-1];
  }//for

}

void multiply(pixel *image_in1, pixel *image_in2){
  int i,j;
  int tmp,trush;
  
  for(i=0;i<PICTURE_VERTICAL_SIZE;i++){
    for(j=0;j<PICTURE_HORIZONTAL_SIZE;j++){
      image_in1[i*PICTURE_HORIZONTAL_SIZE+j]=
	image_in1[i*PICTURE_HORIZONTAL_SIZE+j]*image_in2[i*PICTURE_HORIZONTAL_SIZE+j];
    }
  }
}

void print_image(pixel * test)
{
  int x,y;
  for (y=0; y < PICTURE_VERTICAL_SIZE; y++)
    {
      for (x=0; x < PICTURE_HORIZONTAL_SIZE; x++)
	printf("%3d",(unsigned char)test[PICTURE_HORIZONTAL_SIZE*y+x]);
      printf("\n");
    }
  printf("\n");
}

void print_image_vect(pixel * test)
{
  int x,y;
  for (y=0; y < PICTURE_VERTICAL_SIZE; y++)
    {
      for (x=0; x < PICTURE_HORIZONTAL_SIZE; x++){
	printf("0x%02x",(unsigned char)test[PICTURE_HORIZONTAL_SIZE*y+x]);
	if ( (y != PICTURE_VERTICAL_SIZE-1) | (x != PICTURE_HORIZONTAL_SIZE-1) )
	  printf(",");
      }
      if (y != PICTURE_VERTICAL_SIZE-1)
	printf("\n");
    }
  printf("\n");
}

/********************************************************************/
/*                               MAIN                               */
/********************************************************************/

pixel max_pixel;
pixel image_test_out[PICTURE_VERTICAL_SIZE][PICTURE_HORIZONTAL_SIZE];
pixel image_original[PICTURE_VERTICAL_SIZE][PICTURE_HORIZONTAL_SIZE];

pixel image_back[PICTURE_VERTICAL_SIZE][PICTURE_HORIZONTAL_SIZE];
pixel image_test[PICTURE_VERTICAL_SIZE][PICTURE_HORIZONTAL_SIZE];

main ()
{
  
  int i,j,k;
  
  for (i=0; i < PICTURE_VERTICAL_SIZE; i++) {
    for (j=PICTURE_HORIZONTAL_OFFSET; j < PICTURE_HORIZONTAL_SIZE+PICTURE_HORIZONTAL_OFFSET; j++) {
      image_original[i][j-PICTURE_HORIZONTAL_OFFSET] = Y_test[i][j];
    }
  }
  
  for (i=0; i < PICTURE_VERTICAL_SIZE; i++) {
    for (j=PICTURE_HORIZONTAL_OFFSET; j < PICTURE_HORIZONTAL_SIZE+PICTURE_HORIZONTAL_OFFSET; j++) {
      image_test[i][j-PICTURE_HORIZONTAL_OFFSET] = Y_test[i][j];
    }
  }
  
  for (i=0; i < PICTURE_VERTICAL_SIZE; i++) {
    for (j=PICTURE_HORIZONTAL_OFFSET; j < PICTURE_HORIZONTAL_SIZE+PICTURE_HORIZONTAL_OFFSET; j++) {
      image_back[i][j-PICTURE_HORIZONTAL_OFFSET] = Y_back[i][j];
    }
  }
  
  printf("Starting Motion Detection Application: \n\n");
  
  printf("__attribute__((section(\".heapl2ram\"))) pixel Y_back[%d][%d] = \n{\n",PICTURE_VERTICAL_SIZE,PICTURE_HORIZONTAL_SIZE);
  
  print_image_vect((pixel*)image_back);
  
  printf("};\n");
  
  printf("__attribute__((section(\".heapl2ram\"))) pixel Y_test[%d][%d] = \n{\n",PICTURE_VERTICAL_SIZE,PICTURE_HORIZONTAL_SIZE);
  
  print_image_vect((pixel*)image_test);
  
  printf("};\n");
  
  printf("Subtraction and binarization treshold \n\n");
  sub_image((pixel*)image_test, (pixel*)image_back);
  
  print_image((pixel*)image_test);
  
  max_pixel= max_image((pixel*)image_test);
  
  printf("Max pixel = %d \n\n",max_pixel);
  
  printf("Binarization \n\n");
  binarisation((pixel*)image_test, (int)max_pixel*3/10,1,0);
  
  print_image((pixel*)image_test);
  
  printf("Erosion \n\n");
  erosion((pixel*)image_test,KERNEL_SIZE,(pixel*)image_back);
  print_image((pixel*)image_back);
  
  printf("Dilatation \n\n");
  dilatation((pixel*)image_back,KERNEL_SIZE ,(pixel*)image_test_out);
  print_image((pixel*)image_test_out);
  
  printf("Sobel Horizontal \n\n");
  convolution_rect((pixel*)image_test_out, KERNEL_SIZE, sobel1,(pixel*)image_test);
  val_abs((pixel*)image_test);
  
  print_image((pixel*)image_test);
  
  printf("Sobel Vertical \n\n");
  convolution_rect( (pixel*)image_test_out, KERNEL_SIZE, sobel2,(pixel*)image_back);
  val_abs((pixel*)image_back);
  
  print_image((pixel*)image_back);
  
  printf("Final Sum \n\n");
  sum_image((pixel*)image_test, (pixel*)image_back);
  binarisation((pixel*)image_test, 1,0,1);
  
  print_image((pixel*)image_test);
  
  printf("Final Multiplication \n\n");
  multiply((pixel*)image_test, (pixel*)image_original);
  
  printf("__attribute__((section(\".heapl2ram\"))) pixel Y_golden[%d][%d] = \n{\n",PICTURE_VERTICAL_SIZE,PICTURE_HORIZONTAL_SIZE);
  
  print_image_vect((pixel*)image_test);
  
  printf("};\n\n");
  
  printf("Motion Detection Application Completed\n");
  
}
