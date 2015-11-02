#include "utils.h"
#include "string_lib.h"
#include "bar.h"
#include "timer.h"

#define KERNEL_SIZE 3

//#define DEBUG
#define PROFILING
#define CHECK

typedef short pixel;

//#include "test_5_5.h"
#include "test_20_20.h"

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

static void check_image(pixel *restrict output, pixel *restrict golden) {
  unsigned errors = 0;

  for (unsigned i = 0; i < IMG_HEIGHT * IMG_WIDTH; i++)
    if (output[i] != golden[i])
      errors++;

  if (errors == 0)
    printf("OOOOOOK!!!!!!\n", 0, 0, 0, 0);
  else
    printf("ERROR!!!! %d\n", errors, 0, 0, 0);
}

static void print_image(pixel *restrict test) {
  for (unsigned y = 0; y < IMG_HEIGHT; y++) {
    for (unsigned x = 0; x < IMG_WIDTH; x++)
      printf("%3x", (unsigned char) test[IMG_WIDTH * y + x], 0, 0, 0);

    printf("\n", 0, 0, 0, 0);
  }

  printf("\n", 0, 0, 0, 0);
}

__attribute__((section(".heapl2ram"))) pixel sobel1_l2[9] = {1,2,1,0,0,0,-1,-2,-1};
__attribute__((section(".heapl2ram"))) pixel sobel2_l2[9] = {1,0,-1,2,0,-2,1,0,-1};

__attribute__((section(".heapsram")))  pixel image_back[IMG_HEIGHT][IMG_WIDTH];
__attribute__((section(".heapsram")))  pixel image_test[IMG_HEIGHT][IMG_WIDTH];
__attribute__((section(".heapsram")))  pixel image_original[IMG_HEIGHT][IMG_WIDTH];
__attribute__((section(".heapsram")))  pixel image_test_out[IMG_HEIGHT][IMG_WIDTH];
__attribute__((section(".heapsram")))  pixel sobel1[9];
__attribute__((section(".heapsram")))  pixel sobel2[9];

int main(int argc, char *argv[]) {
  int time = 0;
  int max_pixel = 0;

  printf("Initializing Motion Detection Application Data Structures... \n", 0, 0, 0, 0);

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

  printf("Starting Motion Detection Application \n", 0, 0, 0, 0);

#ifdef PROFILING
  reset_timer();
  start_timer();
#endif

#ifdef DEBUG
  printf("Sub image \n", 0, 0, 0, 0);
#endif

  sub_image((pixel*) image_test, (pixel*) image_back);
  val_abs((pixel*) image_test);

  max_pixel = max_image((pixel*) image_test);

#ifdef DEBUG
  print_image((pixel*) image_test);
  printf("Max pixel: %x \n", max_pixel, 0, 0, 0);
  printf("Binarization \n", 0, 0, 0, 0);
#endif

  binarisation((pixel*) image_test, (int) max_pixel * 3 / 10, 1, 0);

#ifdef DEBUG
  print_image((pixel*) image_test);
  printf("Erosion \n", 0, 0, 0, 0);
#endif

  erosion((pixel*) image_test, (pixel*) image_back);

#ifdef DEBUG
  print_image((pixel*) image_back);
  printf("Dilatation \n", 0, 0, 0, 0);
#endif

  dilatation((pixel*) image_back, (pixel*) image_test_out);

#ifdef DEBUG
  print_image((pixel*) image_test_out);
  printf("Sobel Convolution \n", 0, 0, 0, 0);
#endif

  convolution_rect((pixel*) image_test_out, sobel1, (pixel*) image_test);
  val_abs((pixel*) image_test);

  convolution_rect((pixel*) image_test_out, sobel2, (pixel*) image_back);
  val_abs((pixel*) image_back);

  sum_image((pixel*) image_test, (pixel*) image_back);
  binarisation((pixel*) image_test, 1, 0, 1);

#ifdef DEBUG
  print_image((pixel*) image_test);
  printf("Final Multiplication \n", 0, 0, 0, 0);
#endif

  multiply((pixel*) image_test, (pixel*) image_original);

#ifdef PROFILING
  stop_timer();
  time = get_time();

  printf("Time: %d cycles\n", time, 0, 0, 0);
#endif

#ifdef CHECK
  check_image((pixel*) image_test, (pixel*) Y_golden);
#endif

  printf("Motion Detection Application Complete!!!\n", 0, 0, 0, 0);

  eoc(0);
}
