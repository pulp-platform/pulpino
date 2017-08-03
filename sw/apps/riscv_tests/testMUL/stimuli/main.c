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
#include <stdint.h>
#include <stdlib.h>

int32_t mul(int32_t a, int32_t b);
int32_t mulh(int32_t a, int32_t b);
uint32_t mulhu(uint32_t a, uint32_t b);
uint32_t mulhsu(int32_t a, uint32_t b);
void write_stimuli_int32(FILE* stream, const char* name, int32_t* stim, unsigned int n);
void write_stimuli_uint32(FILE* stream, const char* name, uint32_t* stim, unsigned int n);
void gen_stimuli_mul(FILE* stream, unsigned int n);
void gen_stimuli_mulh(FILE* stream, unsigned int n);
void gen_stimuli_mulhu(FILE* stream, unsigned int n);
void gen_stimuli_mulhsu(FILE* stream, unsigned int n);

int main() {
  int n = 100;
  FILE* stream = fopen("stimuli.h", "w");

  fprintf(stream, "int n_stimuli = %d;\n\n", n);

  gen_stimuli_mul   (stream, n);
  gen_stimuli_mulh  (stream, n);
  gen_stimuli_mulhu (stream, n);
  gen_stimuli_mulhsu(stream, n);

  fclose(stream);
  return 0;
}

int32_t mul(int32_t a, int32_t b) {

  return a * b;
}

int32_t mulh(int32_t a, int32_t b) {
  int64_t al;
  int64_t bl;
  int64_t result;
  int32_t upper;

  al = a;
  bl = b;

  result = al * bl;
  upper = result >> 32;

  return upper;
}

uint32_t mulhu(uint32_t a, uint32_t b) {
  uint64_t al;
  uint64_t bl;
  uint64_t result;
  uint32_t upper;

  al = a;
  bl = b;

  result = al * bl;
  upper = result >> 32;

  return upper;
}

uint32_t mulhsu(int32_t a, uint32_t b) {
  int64_t al;
  uint64_t bl;
  int64_t result;
  int32_t upper;

  al = a;
  bl = b;

  result = al * bl;
  upper = result >> 32;

  return upper;
}

void write_stimuli_uint32(FILE* stream, const char* name, uint32_t* stim, unsigned int n) {
  int i;

  fprintf(stream, "uint32_t %s[] = {\n", name);

  for(i = 0; i < n-1; i++) {
    fprintf(stream, "  %u,\n", stim[i]);
  }

  fprintf(stream, "  %u\n", stim[i]);
  fprintf(stream, "  };\n\n");
}

void write_stimuli_int32(FILE* stream, const char* name, int32_t* stim, unsigned int n) {
  int i;

  fprintf(stream, "int32_t %s[] = {\n", name);

  for(i = 0; i < n-1; i++) {
    fprintf(stream, "  %d,\n", stim[i]);
  }

  fprintf(stream, "  %d\n", stim[i]);
  fprintf(stream, "  };\n\n");
}
void gen_stimuli_mul(FILE* stream, unsigned int n) {
  int i;
  int32_t* stim_a = (int32_t*)malloc(sizeof(int32_t) * n);
  int32_t* stim_b = (int32_t*)malloc(sizeof(int32_t) * n);
  int32_t* exp    = (int32_t*)malloc(sizeof(int32_t) * n);

  for(i = 0; i < n; i++) {
    stim_a[i] = rand();
    stim_b[i] = rand();

    exp[i] = mul(stim_a[i], stim_b[i]);
  }

  write_stimuli_int32(stream, "stim_mul_a",   stim_a, n);
  write_stimuli_int32(stream, "stim_mul_b",   stim_b, n);
  write_stimuli_int32(stream, "stim_mul_exp", exp,    n);

  free(stim_a);
  free(stim_b);
  free(exp);
}
void gen_stimuli_mulh(FILE* stream, unsigned int n) {
  int i;
  int32_t* stim_a = (int32_t*)malloc(sizeof(int32_t) * n);
  int32_t* stim_b = (int32_t*)malloc(sizeof(int32_t) * n);
  int32_t* exp    = (int32_t*)malloc(sizeof(int32_t) * n);

  for(i = 0; i < n; i++) {
    stim_a[i] = rand();
    stim_b[i] = rand();

    exp[i] = mulh(stim_a[i], stim_b[i]);
  }

  write_stimuli_int32(stream, "stim_mulh_a",   stim_a, n);
  write_stimuli_int32(stream, "stim_mulh_b",   stim_b, n);
  write_stimuli_int32(stream, "stim_mulh_exp", exp,    n);

  free(stim_a);
  free(stim_b);
  free(exp);
}

void gen_stimuli_mulhu(FILE* stream, unsigned int n) {
  int i;
  uint32_t* stim_a = (uint32_t*)malloc(sizeof(uint32_t) * n);
  uint32_t* stim_b = (uint32_t*)malloc(sizeof(uint32_t) * n);
  uint32_t* exp    = (uint32_t*)malloc(sizeof(uint32_t) * n);

  for(i = 0; i < n; i++) {
    stim_a[i] = rand();
    stim_b[i] = rand();

    exp[i] = mulhu(stim_a[i], stim_b[i]);
  }

  write_stimuli_uint32(stream, "stim_mulhu_a",   stim_a, n);
  write_stimuli_uint32(stream, "stim_mulhu_b",   stim_b, n);
  write_stimuli_uint32(stream, "stim_mulhu_exp", exp,    n);

  free(stim_a);
  free(stim_b);
  free(exp);
}

void gen_stimuli_mulhsu(FILE* stream, unsigned int n) {
  int i;
  int32_t*  stim_a = (int32_t*)malloc(sizeof(int32_t) * n);
  uint32_t* stim_b = (uint32_t*)malloc(sizeof(uint32_t) * n);
  int32_t*  exp    = (int32_t*)malloc(sizeof(int32_t) * n);

  for(i = 0; i < n; i++) {
    stim_a[i] = rand();
    stim_b[i] = rand();

    exp[i] = mulhsu(stim_a[i], stim_b[i]);
  }

  write_stimuli_int32 (stream, "stim_mulhsu_a",   stim_a, n);
  write_stimuli_uint32(stream, "stim_mulhsu_b",   stim_b, n);
  write_stimuli_int32 (stream, "stim_mulhsu_exp", exp,    n);

  free(stim_a);
  free(stim_b);
  free(exp);
}
