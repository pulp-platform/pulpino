// replacement of riscv_test.h for PULP

#ifndef _RISCV_TEST_H
#define _RISCV_TEST_H


// get core ID and load corresponding stack address into sp
#define FIX_STACK       \
  csrr x1, mhartid;     \
  andi x1, x1, 0x1f;    \
  addi x5, x1, 0x1;     \
  sll  x5, x5, 11;      \
  li   x2, 0x00104000;  \
  add  x2, x2, x5;      \
  addi x2, x2, -4


//-----------------------------------------------------------------------
// Test file setup and include
//-----------------------------------------------------------------------

#define STR_VALUE(x) _STR_VALUE(x)
#define _STR_VALUE(x) #x

#define TEST(file) \
  .text; \
  jal x0, file ## _tests; \
  file ## _name: \
  .string STR_VALUE(file ##.S); \
  .align 4, 0; \
  file ## _tests: \
  FIX_STACK; \
  lui x5, %hi(file ## _name); \
  addi x5, x5, %lo(file ## _name); \
  sw x5, 4(x2);

// e.g. for add.S, it creates the label add_tests, the symbol add_name
// (pointing to the C string "add"), stores a pointer to this string at
// the top of the stack
// this macro is meant to be followed by the content of e.g. add.S

#define TEST_END \
  FIX_STACK; \
  lw x10, 4(x2); \
  jal x1, print_end; \
  asdf: jal x0, asdf


//-----------------------------------------------------------------------
// Test suite setup/teardown
//-----------------------------------------------------------------------

#define RVTEST_RV32U
#define RVTEST_CODE_BEGIN
#define RVTEST_CODE_END


//-----------------------------------------------------------------------
// Test cases, pass/fail logic
//-----------------------------------------------------------------------

#define TESTNUM x28

// generates labels for tests (e.g. test_add_3)
#define _TEST_LABEL(num, suite) test_ ## suite ## _ ## num
#define  TEST_LABEL(num, suite) _TEST_LABEL(num, suite)

// generates pass/fail label names (e.g. test_add_pass/test_add_fail
#define _PASSFAIL_LABEL(pf, suite) test_ ## suite ## _ ## pf
#define  PASSFAIL_LABEL(pf, suite) _PASSFAIL_LABEL(pf, suite)


#define TEST_CASE(testnum, testreg, correctval, code...) \
    TEST_LABEL(testnum, TESTSUITE): \
    code; \
    li  x29, correctval; \
    li  TESTNUM, testnum; \
    bne testreg, x29, PASSFAIL_LABEL(fail, TESTSUITE);


#define TEST_PASSFAIL \
  bne x0, TESTNUM, PASSFAIL_LABEL(pass, TESTSUITE); \
  PASSFAIL_LABEL(fail, TESTSUITE): \
  RVTEST_FAIL; \
  jal x0, PASSFAIL_LABEL(after_pass, TESTSUITE); \
  PASSFAIL_LABEL(pass, TESTSUITE): \
  RVTEST_PASS; \
  PASSFAIL_LABEL(after_pass, TESTSUITE):


#define RVTEST_PASS                                                     \
  FIX_STACK;                                                            \
  addi x10, TESTNUM, 0;                                                 \
  lw x11, 4(x2);                                                        \
  jal x1, print_okay

#define RVTEST_FAIL                                                     \
  FIX_STACK;                                                            \
  addi x10, TESTNUM, 0;                                                 \
  lw x11, 4(x2);                                                        \
  jal x1, print_fail


//-----------------------------------------------------------------------
// Test data section
//-----------------------------------------------------------------------

#define EXTRA_DATA
#define RVTEST_DATA_BEGIN
#define RVTEST_DATA_END

//#define RVTEST_DATA_BEGIN EXTRA_DATA .align 4; .global begin_signature; begin_signature:
//#define RVTEST_DATA_END .align 4; .global end_signature; end_signature:


#endif
