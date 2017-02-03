#include "riscv_test.h"

.global main
main:

// only execute on first core
csrr x1, mhartid
andi x1, x1, 0x1f
bnez x1, finish

// the test pass/fail logic depends on the bne instruction only, so do a quick check first
addi x5, x0, 1
addi x6, x0, 1
bne x5, x6, fail
bne x5, x0, bne_ok
fail: // also catches remaining branches/jumps to the fail label in the tests
.byte 0,0,0,0
bne_ok:


// source files

#undef  TESTSUITE
#define TESTSUITE simple
TEST(simple);
#include "tests/simple.S"

//-----------------------------------------------------------------------
// Arithmetic instructions
//-----------------------------------------------------------------------

#undef  TESTSUITE
#define TESTSUITE add
TEST(add);
#include "tests/add.S"

#undef  TESTSUITE
#define TESTSUITE addi
TEST(addi);
#include "tests/addi.S"

#undef  TESTSUITE
#define TESTSUITE sub
TEST(sub);
#include "tests/sub.S"

#undef  TESTSUITE
#define TESTSUITE and
TEST(and);
#include "tests/and.S"

#undef  TESTSUITE
#define TESTSUITE andi
TEST(andi);
#include "tests/andi.S"

#undef  TESTSUITE
#define TESTSUITE or
TEST(or);
#include "tests/or.S"

#undef  TESTSUITE
#define TESTSUITE ori
TEST(ori);
#include "tests/ori.S"

#undef  TESTSUITE
#define TESTSUITE xor
TEST(xor);
#include "tests/xor.S"

#undef  TESTSUITE
#define TESTSUITE xori
TEST(xori);
#include "tests/xori.S"

#undef  TESTSUITE
#define TESTSUITE sll
TEST(sll);
#include "tests/sll.S"

#undef  TESTSUITE
#define TESTSUITE slli
TEST(slli);
#include "tests/slli.S"

#undef  TESTSUITE
#define TESTSUITE srl
TEST(srl);
#include "tests/srl.S"

#undef  TESTSUITE
#define TESTSUITE srli
TEST(srli);
#include "tests/srli.S"

#undef  TESTSUITE
#define TESTSUITE sra
TEST(sra);
#include "tests/sra.S"

#undef  TESTSUITE
#define TESTSUITE srai
TEST(srai);
#include "tests/srai.S"

#undef  TESTSUITE
#define TESTSUITE slt
TEST(slt);
#include "tests/slt.S"

#undef  TESTSUITE
#define TESTSUITE slti
TEST(slti);
#include "tests/slti.S"


##undef  TESTSUITE
##define TESTSUITE mul
#TEST(mul);
##include "tests/mul.S"

//-----------------------------------------------------------------------
// LUI/AUIPC
//-----------------------------------------------------------------------

#undef  TESTSUITE
#define TESTSUITE lui
TEST(lui);
#include "tests/lui.S"

#undef  TESTSUITE
#define TESTSUITE auipc
TEST(auipc);
#include "tests/auipc.S"


//-----------------------------------------------------------------------
// Jumps
//-----------------------------------------------------------------------

/*
  Test uses custom labels -> incompatible with rest of tests

#undef  TESTSUITE
#define TESTSUITE jal
TEST(jal);
#include "tests/jal.S"

#undef  TESTSUITE
#define TESTSUITE jalr
TEST(jalr);
#include "tests/jalr.S"

#undef  TESTSUITE
#define TESTSUITE j
TEST(j);
#include "tests/j.S"

*/
  .text
finish:
  FIX_STACK;

  // synch barrier
  //li x10, 0
  //jal x1, wait_barrier_event_buff

  jal print_test_summary
  jal x0, (. - 4)
