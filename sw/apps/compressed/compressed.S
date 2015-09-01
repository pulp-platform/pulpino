// can be used to check result of RVC instr with shared rd/rs1 and immediate
#define TEST_RI(rvc_instr, rv_instr, r_value, imm_value) \
  li a0, r_value; \
  rv_instr  a1, a0, imm_value; \
  .option rvc; \
  rvc_instr a0, imm_value; \
  .option norvc; \
  bne a0, a1, fail;

// can be used to check result of RVC instr with shared rd/rs1 and rs2
#define TEST_RR(rvc_instr, rv_instr, r1_value, r2_value) \
  li a0, r1_value; \
  li a2, r2_value; \
  rv_instr  a1, a0, a2; \
  .option rvc; \
  rvc_instr a0, a2; \
  .option norvc; \
  bne a0, a1, fail;

// can be used to check result of RVC instr separate rd, rs1 and rs2
#define TEST_RRI(rvc_instr, rv_instr, r_value, imm_value) \
  li a2, r_value; \
  .option rvc; \
  rvc_instr a0, a2, imm_value; \
  .option norvc; \
  rv_instr  a1, a2, imm_value; \
  bne a0, a1, fail;


// --------------------------------------------------------------------------------
// ---  Entrypoint
// --------------------------------------------------------------------------------
.option norvc
.global main
main:
  addi sp, sp, -4
  sw x1, 0(sp)

  // initialize UART and send hello
  li a0, 0
  li a1, 0
  jal x1, uart_set_cfg
  la a0, hello_str
  li a1, 11 // TODO: FixMe replace with hello_size
  jal x1, uart_send
  jal x1, uart_wait_tx_done

  jal x1, tests

  la a0, pass_str
  li a1, 5
  jal x1, uart_send

exit:
  jal x1, uart_wait_tx_done

  // return 0
  li a0, 0
  lw x1, 0(sp)
  addi sp, sp, 4
  jalr x0, x1

fail:
  la a0, fail_str
  li a1, 5
  jal x1, uart_send
  j exit


// --------------------------------------------------------------------------------
// ---  Tests
// --------------------------------------------------------------------------------
.global tests
tests:

  // c.lwsp
  // c.swsp

  // c.lw
  // c.sw

  // c.j
  // c.jal
  // c.jr
  // c.jalr

  // c.beqz
  // c.bnez

  // c.li
  // c.lui

  // c.addi
  li x31, 12345
  c.addi x31, 31
  li t0, 12345+31
  bne t0, x31, fail

  /*
  // c.addi16sp
  mv t0, sp
  c.addi16sp -512
  addi t0, t0, -512
  bne t0, sp, fail
  addi sp, sp, 512

  // c.addi4spn
  c.addi4spn t0, 512
  addi t1, sp, 512
  bne t0, t1, fail
  */

  // c.slli
  TEST_RI(c.slli, slli, 0xff, 8)

  // c.mv
  //TEST_RR(c.mv, add, 54321, 12345)
  // c.add
  TEST_RR(c.add, add, 54321, 12345)
  // c.sub
  TEST_RR(c.sub, sub, 54321, 12345)

  // c.nop

  // c.ebreak

  // Extended RVC instructions

  // c.bltz
  // c.bgez

  // c.andi
  TEST_RI(c.andi, andi, 0xffff, 15)

  // c.addin
  TEST_RRI(c.addin, addi, 1023, 3)
  // c.andin
  TEST_RRI(c.andin, andi, 1023, 3)
  // c.orin
  TEST_RRI(c.orin, ori, 1024, 3)
  // c.xorin
  TEST_RRI(c.xorin, xori, 1023, 3)

  // c.srli
  TEST_RI(c.srli, srli, 1023, 7)
  // c.srai
  TEST_RI(c.srai, srai, 4095, 7)

  // return to main
  jr x1



// --------------------------------------------------------------------------------
// ---  Data
// --------------------------------------------------------------------------------
.data

hello_str:
  .string "RVC Test:\n\n"
  .set hello_size, .-hello_str

pass_str:
  .string "PASS\n"
fail_str:
  .string "FAIL\n"

