// we assume everything in here is compiled without RVC, except the parts we mark explicitely

// can be used to check result of RVC instr with separate rd and rs1
#define TEST_R(rvc_instr, rv_instr, r_value) \
  .data; \
rvc_string ##rvc_instr: \
  .string #rvc_instr; \
  .text; \
  lui a0,      %hi(rvc_string##rvc_instr); \
  addi a0, a0, %lo(rvc_string##rvc_instr); \
  \
  li a3, r_value; \
  rv_instr  a2, x0, a3; \
  .option rvc; \
  rvc_instr a1, a3; \
  .option norvc; \
  jal helper_check;

// can be used to check result of RVC instr with just one immediate and rD
#define TEST_I(rvc_instr, rv_instr, imm_value) \
  .data; \
rvc_string ##rvc_instr: \
  .string #rvc_instr; \
  .text; \
  lui a0,      %hi(rvc_string##rvc_instr); \
  addi a0, a0, %lo(rvc_string##rvc_instr); \
  \
  rv_instr  a2, imm_value; \
  .option rvc; \
  rvc_instr a1, imm_value; \
  .option norvc; \
  jal helper_check;

// can be used to check result of RVC instr with shared rd/rs1 and immediate
#define TEST_RI(rvc_instr, rv_instr, r_value, imm_value) \
  .data; \
rvc_string ##rvc_instr: \
  .string #rvc_instr; \
  .text; \
  lui a0,      %hi(rvc_string##rvc_instr); \
  addi a0, a0, %lo(rvc_string##rvc_instr); \
  \
  li a1, r_value; \
  rv_instr  a2, a1, imm_value; \
  .option rvc; \
  rvc_instr a1, imm_value; \
  .option norvc; \
  jal helper_check;

// can be used to check result of RVC instr with shared rd/rs1 and rs2
#define TEST_RR(rvc_instr, rv_instr, r1_value, r2_value) \
  .data; \
rvc_string ##rvc_instr: \
  .string #rvc_instr; \
  .text; \
  lui a0,      %hi(rvc_string##rvc_instr); \
  addi a0, a0, %lo(rvc_string##rvc_instr); \
  \
  li a1, r1_value; \
  li a3, r2_value; \
  rv_instr  a2, a1, a3; \
  .option rvc; \
  rvc_instr a1, a3; \
  .option norvc; \
  jal helper_check;

// can be used to check result of RVC instr with shared rd/rs2 and rs1
#define TEST_RRev(rvc_instr, rv_instr, r1_value, r2_value) \
  .data; \
rvc_string ##rvc_instr: \
  .string #rvc_instr; \
  .text; \
  lui a0,      %hi(rvc_string##rvc_instr); \
  addi a0, a0, %lo(rvc_string##rvc_instr); \
  \
  li a1, r1_value; \
  li a3, r2_value; \
  rv_instr  a2, a3, a1; \
  .option rvc; \
  rvc_instr a1, a3; \
  .option norvc; \
  jal helper_check;


// can be used to check result of RVC instr separate rd, rs1 and rs2
#define TEST_RR3(rvc_instr, rv_instr, r1_value, r2_value) \
  .data; \
rvc_string ##rvc_instr: \
  .string #rvc_instr; \
  .text; \
  lui a0,      %hi(rvc_string##rvc_instr); \
  addi a0, a0, %lo(rvc_string##rvc_instr); \
  \
  li a3, r1_value; \
  li a4, r2_value; \
  .option rvc; \
  rvc_instr a2, a3, a4; \
  .option norvc; \
  rv_instr  a1, a3, a4; \
  jal helper_check;

// can be used to check result of RVC instr separate rd, rs1 and an immediate
#define TEST_RRI(rvc_instr, rv_instr, r_value, imm_value) \
  .data; \
rvc_string ##rvc_instr: \
  .string #rvc_instr; \
  .text; \
  lui a0,      %hi(rvc_string##rvc_instr); \
  addi a0, a0, %lo(rvc_string##rvc_instr); \
  \
  li a3, r_value; \
  .option rvc; \
  rvc_instr a2, a3, imm_value; \
  .option norvc; \
  rv_instr  a1, a3, imm_value; \
  jal helper_check;

// used for c.addi16sp
// this one is more a fake than anything useful
#define TEST_16SP(rvc_instr, rv_instr, imm_value) \
  .data; \
rvc_string ##rvc_instr: \
  .string #rvc_instr; \
  .text; \
  lui a0,      %hi(rvc_string##rvc_instr); \
  addi a0, a0, %lo(rvc_string##rvc_instr); \
  \
  .option rvc; \
  rvc_instr sp, imm_value; \
  .option norvc; \
  rv_instr  sp, sp, -imm_value; \
  li a1, 0; \
  li a2, 0; \
  jal helper_check;

// used for c.add4spn
#define TEST_4SPN(rvc_instr, rv_instr, imm_value) \
  .data; \
rvc_string ##rvc_instr: \
  .string #rvc_instr; \
  .text; \
  lui a0,      %hi(rvc_string##rvc_instr); \
  addi a0, a0, %lo(rvc_string##rvc_instr); \
  \
  .option rvc; \
  rvc_instr a1, sp, imm_value; \
  .option norvc; \
  rv_instr  a2, sp, imm_value; \
  jal helper_check;



// used for c.add4spn
#define TEST_BR_TAKE(rvc_instr, r_value) \
  .data; \
rvc_string ##rvc_instr: \
  .string #rvc_instr; \
  .text; \
  lui a0,      %hi(rvc_string##rvc_instr); \
  addi a0, a0, %lo(rvc_string##rvc_instr); \
  \
  li a1, r_value; \
  .option rvc; \
  rvc_instr a1, rvc_instr##_take; \
  .option norvc; \
  jal helper_error; \
rvc_instr##_take:;

#define TEST_BR_NOT_TAKE(rvc_instr, r_value) \
  .data; \
rvc_string2 ##rvc_instr: \
  .string #rvc_instr; \
  .text; \
  lui a0,      %hi(rvc_string2##rvc_instr); \
  addi a0, a0, %lo(rvc_string2##rvc_instr); \
  \
  li a1, r_value; \
  .option rvc; \
  rvc_instr a1, rvc_instr##_not_take; \
  .option norvc; \
  j rvc_instr##_done; \
rvc_instr##_not_take:; \
  jal helper_error; \
rvc_instr##_done:;


#define TEST_J(rvc_instr, should_be_zero) \
  .data; \
rvc_string ##rvc_instr: \
  .string #rvc_instr; \
  .text; \
  lui a0,      %hi(rvc_string##rvc_instr); \
  addi a0, a0, %lo(rvc_string##rvc_instr); \
  \
  li ra, 0; \
  li a1, should_be_zero; \
  .option rvc; \
  rvc_instr rvc_instr##_jt; \
  .option norvc; \
  jal helper_error; \
rvc_instr##_jt:; \
  mv a2, ra; \
  jal helper_link_check

#define TEST_JR(rvc_instr, should_be_zero) \
  .data; \
rvc_string ##rvc_instr: \
  .string #rvc_instr; \
  .text; \
  lui a0,      %hi(rvc_string##rvc_instr); \
  addi a0, a0, %lo(rvc_string##rvc_instr); \
  \
  li ra, 0; \
  li a1, should_be_zero; \
  lui  a2,     %hi(rvc_instr##_jt); \
  addi a2, a2, %lo(rvc_instr##_jt); \
  .option rvc; \
  rvc_instr a2; \
  .option norvc; \
  jal helper_error; \
rvc_instr##_jt:; \
  mv a2, ra; \
  jal helper_link_check

// --------------------------------------------------------------------------------
// ---  Entrypoint
// --------------------------------------------------------------------------------
.option norvc
.text;
// --------------------------------------------------------------------------------
// ---  Tests
// --------------------------------------------------------------------------------
.global compressed_vs_normal_tests
compressed_vs_normal_tests:
  addi sp, sp, -4
  sw x1, 0(sp)

  // TODO: what is the syntax of those instructions?!?
  // c.lwsp
  // c.swsp

  // c.lw
  // c.sw

  //----------------------------------------------------------------------------
  // arithmetic
  //----------------------------------------------------------------------------

  // c.li
  TEST_I(c.li, li, 17)

  // c.lui
  TEST_I(c.lui, lui, 13)

  // c.addi
  TEST_RI(c.addi, addi, 1337, 0xb)

  // c.add3
  //TEST_RR3(c.add3, add, 123456, 1236247)

  // c.add
  TEST_RR(c.add, add, 54321, 12345)

  // c.addi16sp
  TEST_16SP(c.addi16sp, add, 0x20)

  // c.addi4spn
  TEST_4SPN(c.addi4spn, add, 0x20)

  // c.addin
  //TEST_RRI(c.addin, addi, 1023, 3)

  // c.mv
  TEST_R(c.mv, add, 54321)

  // c.sub
  TEST_RR(c.sub, sub, 54321, 12345)

  // c.sub3
  //TEST_RR3(c.sub3, sub, 54321, 12345)

  //----------------------------------------------------------------------------
  // logical
  //----------------------------------------------------------------------------

  // c.andi
  TEST_RI(c.andi, andi, 0xffff, 15)

  // c.andin
  //TEST_RRI(c.andin, andi, 1023, 3)

  // c.and3
  //TEST_RR3(c.and3, and, 67611, 6678672)

  // c.orin
  //TEST_RRI(c.orin, ori, 1023, 3)

  // c.or3
  //TEST_RR3(c.or3, or, 67611, 6678672)

  // c.xorin
  //TEST_RRI(c.xorin, xori, 1023, 3)

  // c.xor
  TEST_RR(c.xor, xor, 1023, 3345)

  //----------------------------------------------------------------------------
  // shifts
  //----------------------------------------------------------------------------

  // c.slli
  TEST_RI(c.slli, slli, 0xff, 8)

  // c.sll
  TEST_RR(c.sll, sll, 0xff, 3)

  // c.sllr
  //TEST_RRev(c.sllr, sll, 0xff, 3)

  // c.srli
  TEST_RI(c.srli, srli, 1023, 7)

  // c.srl
  TEST_RR(c.srl, srl, 0xff, 3)

  // c.srlr
  //TEST_RRev(c.srlr, srl, 0xff, 3)

  // c.srai
  TEST_RI(c.srai, srai, 4095, 7)

  // c.sra
  //TEST_RR(c.sra, sra, 4095, 7)


  // c.sltr
  //TEST_RRev(c.sltr, slt, 3446, 4656756)

  // c.sltur
  //TEST_RRev(c.sltur, sltu, 3446, 4656756)

  // c.slt
  //TEST_RR(c.slt, slt, 3446, 4656756)

  // c.sltu
  //TEST_RR(c.sltu, sltu, 3446, 4656756)

  // c.nop
  // no point in testing it... no visible effect

  // c.ebreak

  //----------------------------------------------------------------------------
  // branches and jumps
  //----------------------------------------------------------------------------

  // c.beqz
  TEST_BR_TAKE(c.beqz, 0x0);
  TEST_BR_NOT_TAKE(c.beqz, 5);

  // c.bnez
  TEST_BR_TAKE(c.bnez, 1);
  TEST_BR_NOT_TAKE(c.bnez, 0x0);

  // c.bltz
  //TEST_BR_TAKE(c.bltz, -5);
  //TEST_BR_NOT_TAKE(c.bltz, 5);

  // c.bgez
  //TEST_BR_TAKE(c.bgez, 0);
  //TEST_BR_NOT_TAKE(c.bgez, -1);

  // c.j
  TEST_J(c.j, 1)
  // c.jal
  TEST_J(c.jal, 0)
  // c.jr
  TEST_JR(c.jr, 1)
  // c.jalr
  TEST_JR(c.jalr, 0)


  // return to calling function
  li a0, 0
  lw x1, 0(sp)
  addi sp, sp, 4
  jalr x0, x1

