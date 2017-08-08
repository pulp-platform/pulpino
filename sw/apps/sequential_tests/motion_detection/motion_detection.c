// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "utils.h"
#include "string_lib.h"
#include "bar.h"
#include "timer.h"
#include "motion_detection.h"

#define KERNEL_SIZE 3

//#define DEBUG
#define PROFILE

#ifdef DOTP
  #warning DOTP has been defined
#endif

typedef short pixel;

#include "img_10_10.h"
//#include "img_20_20.h"
//#include "img_40_40.h"

void check_motion_detection         (testresult_t *result, void (*start)(), void (*stop)());
void check_motion_detection_dotp    (testresult_t *result, void (*start)(), void (*stop)());
void Conv3x3_fast(pixel*restrict out, pixel*restrict in, pixel*restrict kernel);


testcase_t testcases[] = {
  { .name = "motion_detection"      , .test = check_motion_detection       },
#ifdef DOTP
  { .name = "motion_detection_dotp" , .test = check_motion_detection_dotp  },
#endif
  {0, 0}
};

#include "conv_fast.c"

static void val_abs(pixel *restrict image_in) {
  for (unsigned i = 0; i < IMG_HEIGHT; i++)
    for (unsigned j = 0; j < IMG_WIDTH; j++)
      if (image_in[i * IMG_WIDTH + j] < 0)
        image_in[i * IMG_WIDTH + j] = -image_in[i * IMG_WIDTH + j];
}

static void sum_image(pixel *restrict image_in1, pixel *restrict image_in2) {
  for (unsigned i = 0; i < IMG_HEIGHT; i++)
    for (unsigned j = 0; j < IMG_WIDTH; j++)
      image_in1[i * IMG_WIDTH + j] =
            (image_in1[i * IMG_WIDTH + j])+(image_in2[i * IMG_WIDTH + j]);
}

static void sub_image(pixel *restrict image_in1, pixel *restrict image_in2) {
  for (unsigned i = 0; i < IMG_HEIGHT; i++)
    for (unsigned j = 0; j < IMG_WIDTH; j++)
      image_in1[i * IMG_WIDTH + j] =
            image_in1[i * IMG_WIDTH + j] - image_in2[i * IMG_WIDTH + j];
}

static pixel max_image(pixel *restrict image_in) {
  pixel max_pix = 0;

  for (unsigned i = 0; i < IMG_HEIGHT; i++)
    for (unsigned j = 0; j < IMG_WIDTH; j++)
      if (image_in[i * IMG_WIDTH + j] > max_pix)
        max_pix = image_in[i * IMG_WIDTH + j];

  return max_pix;
}

static pixel min_image(pixel *restrict image_in) {
  pixel min_pix = 0;

  for (unsigned i = 0; i < IMG_HEIGHT; i++)
    for (unsigned j = 0; j < IMG_WIDTH; j++)
      if (image_in[i * IMG_WIDTH + j] < min_pix)
        min_pix = image_in[i * IMG_WIDTH + j];

  return min_pix;
}

static void binarisation(pixel *restrict image_in, pixel thresh,
                         pixel val_max, pixel val_min) {
  for (unsigned i = 0; i < IMG_HEIGHT; i++)
    for (unsigned j = 0; j < IMG_WIDTH; j++)
      if (image_in[i * IMG_WIDTH + j] < thresh)
        image_in[i * IMG_WIDTH + j] = val_min;
      else
        image_in[i * IMG_WIDTH + j] = val_max;
}

static void erosion(pixel *restrict image_in, pixel *restrict image_out) {
  for (unsigned x = 1; x < IMG_HEIGHT - 1; x++) {
    for (unsigned y = 1; y < IMG_WIDTH - 1; y++) {
      pixel min = 255;

      for (unsigned i = 0; i < KERNEL_SIZE; i++)
        for (unsigned j = 0; j < KERNEL_SIZE; j++)
          if (image_in[(x-1) * IMG_WIDTH + (y-1) + (i*IMG_WIDTH + j)] < min)
            min = image_in[(x-1) * IMG_WIDTH + (y-1) + (i*IMG_WIDTH + j)];

      image_out[x*IMG_WIDTH + y] = min;
    }
  }

  /* Boundary Copy */
  for (unsigned k = 0; k < IMG_WIDTH; k++) {
    image_out[k] = image_in[k];
    image_out[(IMG_HEIGHT-1) * IMG_WIDTH + k] =
        image_in[(IMG_HEIGHT-1) * IMG_WIDTH + k];
  }

  for (unsigned k = 0; k < IMG_HEIGHT; k++) {
    image_out[k*IMG_WIDTH] = image_in[k*IMG_WIDTH];
    image_out[k*IMG_WIDTH + IMG_WIDTH - 1] =
        image_in[k*IMG_WIDTH + IMG_WIDTH - 1];
  }
}

static void dilatation(pixel *restrict image_in, pixel *restrict image_out) {
  for (unsigned x = 1; x < IMG_HEIGHT - 1; x++) {
    for (unsigned y = 1; y < IMG_WIDTH - 1; y++) {
      pixel max = 0;

      for (unsigned i = 0; i < KERNEL_SIZE; i++)
        for (unsigned j = 0; j < KERNEL_SIZE; j++)
          if (image_in[(x-1) * IMG_WIDTH + (y-1) + (i*IMG_WIDTH + j)] > max)
            max = image_in[(x-1) * IMG_WIDTH + (y-1) + (i*IMG_WIDTH + j)];

      image_out[x*IMG_WIDTH + y] = max;
    }
  }

  /* Boundary Copy */
  for (unsigned k = 0; k < IMG_WIDTH; k++) {
    image_out[k] = image_in[k];
    image_out[(IMG_HEIGHT-1) * IMG_WIDTH + k] =
        image_in[(IMG_HEIGHT-1) * IMG_WIDTH + k];
  }
  for (unsigned k = 0; k < IMG_HEIGHT; k++) {
    image_out[k*IMG_WIDTH] = image_in[k*IMG_WIDTH];
    image_out[k*IMG_WIDTH + IMG_WIDTH - 1] =
        image_in[k*IMG_WIDTH + IMG_WIDTH - 1];
  }
}

static void convolution_rect(pixel *restrict image_in, pixel *restrict kernel,
                             pixel *restrict image_out) {
  for (unsigned x = 1; x < IMG_HEIGHT - 1; x++) {
    for (unsigned y = 1; y < IMG_WIDTH - 1; y++) {
      image_out[x * IMG_WIDTH + y] = 0;

      for (unsigned i = 0; i < KERNEL_SIZE; i++)
        for (unsigned j = 0; j < KERNEL_SIZE; j++)
          image_out[x*IMG_WIDTH + y] +=
                  image_in[(x-1) * IMG_WIDTH + (y-1) + i*IMG_WIDTH + j] *
                  kernel[i*KERNEL_SIZE + j];
    }
  }

  for (unsigned k = 0; k < IMG_WIDTH; k++) {
    image_out[k] = image_in[k];
    image_out[(IMG_HEIGHT-1) * IMG_WIDTH + k] =
        image_in[(IMG_HEIGHT-1) * IMG_WIDTH + k];
  }

  for (unsigned k = 0; k < IMG_HEIGHT; k++) {
    image_out[k*IMG_WIDTH] = image_in[k*IMG_WIDTH];
    image_out[k*IMG_WIDTH + IMG_WIDTH - 1] =
        image_in[k*IMG_WIDTH + IMG_WIDTH - 1];
  }
}

static void multiply(pixel *restrict image_in1, pixel *restrict image_in2) {
  for (unsigned i = 0; i < IMG_HEIGHT; i++)
    for (unsigned j = 0; j < IMG_WIDTH; j++)
      image_in1[i*IMG_WIDTH + j] =
            image_in1[i*IMG_WIDTH + j] * image_in2[i*IMG_WIDTH + j];
}

int check_image(pixel *restrict output, pixel *restrict golden) {
  unsigned errors = 0;
  
  for (unsigned i = 0; i < IMG_HEIGHT * IMG_WIDTH; i++)
    if (output[i] != golden[i])
      errors++;
  
  return errors;
}

static void print_image(pixel *restrict test) {
  for (unsigned y = 0; y < IMG_HEIGHT; y++) {
    for (unsigned x = 0; x < IMG_WIDTH; x++)
      printf("%3x", (unsigned char) test[IMG_WIDTH * y + x]);

    printf("\n");
  }

  printf("\n");
}

__attribute__((section(".heapl2ram"))) pixel sobel1_l2[9] = {1,2,1,0,0,0,-1,-2,-1};
__attribute__((section(".heapl2ram"))) pixel sobel2_l2[9] = {1,0,-1,2,0,-2,1,0,-1};

__attribute__((section(".heapsram")))  pixel image_back[IMG_HEIGHT][IMG_WIDTH];
__attribute__((section(".heapsram")))  pixel image_test[IMG_HEIGHT][IMG_WIDTH];
__attribute__((section(".heapsram")))  pixel image_original[IMG_HEIGHT][IMG_WIDTH];
__attribute__((section(".heapsram")))  pixel image_test_out[IMG_HEIGHT][IMG_WIDTH];
__attribute__((section(".heapsram")))  pixel sobel1[9];
__attribute__((section(".heapsram")))  pixel sobel2[9];

int main()
{
  return run_suite(testcases);
}

void check_motion_detection(testresult_t *result, void (*start)(), void (*stop)()) {

  init_motion_detection();
  printf("Starting Motion Detection Application \n");

#ifdef PROFILE
  perf_reset();
  perf_enable_id(EVENT_ID);
#endif
  start();
  motion_detection(0);
  stop();
#ifdef PROFILE
  perf_stop();
  printf("Perf: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#endif
  result->errors = check_image((pixel*) image_test, (pixel*) Y_golden);
  uart_wait_tx_done();
}

void check_motion_detection_dotp(testresult_t *result, void (*start)(), void (*stop)()) {

  init_motion_detection();
  printf("Starting Motion Detection Application with dotp \n");

#ifdef PROFILE
  perf_reset();
  perf_enable_id(EVENT_ID);
#endif
  start();
  motion_detection(1);
  stop();
#ifdef PROFILE
  perf_stop();
  printf("Perf: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#endif
  result->errors = check_image((pixel*) image_test, (pixel*) Y_golden);
  uart_wait_tx_done();
}

void init_motion_detection(){
  printf("Initializing Motion Detection Application Data Structures... \n");

  // INITIALIZING SOBEL VECTORS
  for (unsigned i = 0; i < 9; i++) {
    sobel1[i] = sobel1_l2[i];
    sobel2[i] = sobel2_l2[i];
  }

  // INITIALIZING Y_TEST
  for (unsigned  i = 0; i < IMG_HEIGHT; i++)
    for (unsigned  j = 0; j < IMG_WIDTH; j++)
      image_test[i][j] = Y_test[i][j];

#ifdef DEBUG
  print_image((pixel*) image_test);
#endif

  // INITIALIZING Y_BACK
  for (unsigned  i = 0; i < IMG_HEIGHT; i++)
    for (unsigned  j = 0; j < IMG_WIDTH; j++)
      image_back[i][j] = Y_back[i][j];

#ifdef DEBUG
  print_image((pixel*) image_back);
#endif

  // INITIALIZING Y_TEST
  for (unsigned i = 0; i < IMG_HEIGHT; i++)
    for (unsigned j = 0; j < IMG_WIDTH; j++)
      image_original[i][j] = Y_test[i][j];

#ifdef DEBUG
  print_image((pixel*) image_original);
#endif

}


void motion_detection(int fast_conv) {
  int time = 0;
  int max_pixel = 0;


#ifdef DEBUG
  printf("Sub image \n");
#endif

  sub_image((pixel*) image_test, (pixel*) image_back);
  val_abs((pixel*) image_test);

  max_pixel = max_image((pixel*) image_test);

#ifdef DEBUG
  print_image((pixel*) image_test);
  printf("Max pixel: %x \n", max_pixel);
  printf("Binarization \n");
#endif

  binarisation((pixel*) image_test, (int) max_pixel * 3 / 10, 1, 0);

#ifdef DEBUG
  print_image((pixel*) image_test);
  printf("Erosion \n");
#endif

  erosion((pixel*) image_test, (pixel*) image_back);

#ifdef DEBUG
  print_image((pixel*) image_back);
  printf("Dilatation \n");
#endif

  dilatation((pixel*) image_back, (pixel*) image_test_out);

#ifdef DEBUG
  print_image((pixel*) image_test_out);
  printf("Sobel Convolution \n");
#endif

  if (fast_conv)
    Conv3x3_fast((pixel*) image_test_out, (pixel*) image_test, sobel1);
  else
    convolution_rect((pixel*) image_test_out, sobel1, (pixel*) image_test);

  val_abs((pixel*) image_test);
 
  if (fast_conv)
    Conv3x3_fast((pixel*) image_test_out, (pixel*) image_back, sobel2);
  else
    convolution_rect((pixel*) image_test_out, sobel2, (pixel*) image_back);

  val_abs((pixel*) image_back);

  sum_image((pixel*) image_test, (pixel*) image_back);
  binarisation((pixel*) image_test, 1, 0, 1);

#ifdef DEBUG
  print_image((pixel*) image_test);
  printf("Final Multiplication \n");
#endif

  multiply((pixel*) image_test, (pixel*) image_original);

}
