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
#include <stdlib.h>
#include <stdint.h>

struct stimuli_t {
  uint32_t a;
  uint32_t b;
  uint32_t exp;
};

int do_div(int a, int b) {
  int exp;

  if (b == 0)
    exp = -1;
  else if (a == 0x80000000 && b == 0xFFFFFFFF)
    exp = 0x80000000;
  else
    exp = a/b;

  return exp;
}

unsigned int do_divu(unsigned int a, unsigned int b) {
  unsigned int exp;

  if (b == 0)
    exp = 0xFFFFFFFF;
  else
    exp = a/b;

  return exp;
}

int do_rem(int a, int b) {
  int exp;

  if (b == 0)
    exp = a;
  else if (a == 0x80000000 && b == -1)
    exp = 0;
  else
    exp = a%b;

  return exp;
}

unsigned int do_remu(unsigned int a, unsigned int b) {
  unsigned int exp;

  if (b == 0)
    exp = a;
  else
    exp = a%b;

  return exp;
}

void randomize_div(struct stimuli_t* stim, int count) {
  for(int i = 0; i < count; i++) {
    struct stimuli_t* pStim = &stim[i];

    int a = rand();
    int b = rand();

    if (rand() & 0x1)
      a = -a;

    if (rand() & 0x1)
      b = -b;

    pStim->a   = a;
    pStim->b   = b;
    pStim->exp = do_div(a, b);
  }
}

void randomize_divu(struct stimuli_t* stim, int count) {
  for(int i = 0; i < count; i++) {
    struct stimuli_t* pStim = &stim[i];

    unsigned int a = rand();
    unsigned int b = rand();

    pStim->a   = a;
    pStim->b   = b;
    pStim->exp = do_divu(a, b);
  }
}

void randomize_rem(struct stimuli_t* stim, int count) {
  for(int i = 0; i < count; i++) {
    struct stimuli_t* pStim = &stim[i];

    int a = rand();
    int b = rand();

    if (rand() & 0x1)
      a = -a;

    if (rand() & 0x1)
      b = -b;

    pStim->a   = a;
    pStim->b   = b;
    pStim->exp = do_rem(a, b);
  }
}

void randomize_remu(struct stimuli_t* stim, int count) {
  for(int i = 0; i < count; i++) {
    struct stimuli_t* pStim = &stim[i];

    unsigned int a = rand();
    unsigned int b = rand();

    pStim->a   = a;
    pStim->b   = b;
    pStim->exp = do_remu(a, b);
  }
}

void write_div(FILE* f, struct stimuli_t* stim, int count) {
  fprintf(f, "#define DIV_STIMS %u\n", count);
  fprintf(f, "struct div_stim_t {\n");
  fprintf(f, "     int a;\n");
  fprintf(f, "     int b;\n");
  fprintf(f, "     int exp;\n");
  fprintf(f, "};\n\n");
  fprintf(f, "struct div_stim_t div_stim[%u] = {\n", count);

  for(int i = 0; i < count; i++) {
    struct stimuli_t* pStim = &stim[i];
    fprintf(f, "          {%d, %d, %d},\n", pStim->a, pStim->b, pStim->exp);
  }

  fprintf(f, "};\n\n\n");
}

void write_divu(FILE* f, struct stimuli_t* stim, int count) {
  fprintf(f, "#define DIVU_STIMS %u\n", count);
  fprintf(f, "struct divu_stim_t {\n");
  fprintf(f, "     unsigned int a;\n");
  fprintf(f, "     unsigned int b;\n");
  fprintf(f, "     unsigned int exp;\n");
  fprintf(f, "};\n\n");
  fprintf(f, "struct divu_stim_t divu_stim[%u] = {\n", count);

  for(int i = 0; i < count; i++) {
    struct stimuli_t* pStim = &stim[i];
    fprintf(f, "          {%u, %u, %u},\n", pStim->a, pStim->b, pStim->exp);
  }

  fprintf(f, "};\n\n\n");
}

void write_rem(FILE* f, struct stimuli_t* stim, int count) {
  fprintf(f, "#define REM_STIMS %u\n", count);
  fprintf(f, "struct div_stim_t rem_stim[%u] = {\n", count);

  for(int i = 0; i < count; i++) {
    struct stimuli_t* pStim = &stim[i];
    fprintf(f, "          {%d, %d, %d},\n", pStim->a, pStim->b, pStim->exp);
  }

  fprintf(f, "};\n\n\n");
}

void write_remu(FILE* f, struct stimuli_t* stim, int count) {
  fprintf(f, "#define REMU_STIMS %u\n", count);
  fprintf(f, "struct divu_stim_t remu_stim[%u] = {\n", count);

  for(int i = 0; i < count; i++) {
    struct stimuli_t* pStim = &stim[i];
    fprintf(f, "          {%u, %u, %u},\n", pStim->a, pStim->b, pStim->exp);
  }

  fprintf(f, "};\n\n\n");
}

int main() {
  struct stimuli_t stimuli_div[20];
  struct stimuli_t stimuli_divu[20];
  struct stimuli_t stimuli_rem[20];
  struct stimuli_t stimuli_remu[20];

  randomize_div (stimuli_div,  20);
  randomize_divu(stimuli_divu, 20);
  randomize_rem (stimuli_rem,  20);
  randomize_remu(stimuli_remu, 20);

  FILE* f;
  f = fopen("stimuli.h", "w");

  write_div (f, stimuli_div,  20);
  write_divu(f, stimuli_divu, 20);
  write_rem (f, stimuli_rem,  20);
  write_remu(f, stimuli_remu, 20);

  fclose(f);

  return 0;
}
