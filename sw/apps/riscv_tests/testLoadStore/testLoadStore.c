////////////////////////////////////////////////////////////////////////////////
//
// This test tries to verify post-increment and register-register loads and
// stores.
//
// The following instructions are used:
//
//  - p.lw  rd, I(rs1!)
//  - p.lhu rd, I(rs1!)
//  - p.lh  rd, I(rs1!)
//  - p.lbu rd, I(rs1!)
//  - p.lb  rd, I(rs1!)
//
//  - p.lw  rd, rs2(rs1)
//  - p.lhu rd, rs2(rs1)
//  - p.lh  rd, rs2(rs1)
//  - p.lbu rd, rs2(rs1)
//  - p.lb  rd, rs2(rs1)
//
//  - p.lw  rd, rs2(rs1!)
//  - p.lhu rd, rs2(rs1!)
//  - p.lh  rd, rs2(rs1!)
//  - p.lbu rd, rs2(rs1!)
//  - p.lb  rd, rs2(rs1!)
//
//  - p.sw I(rs1!), rs2
//  - p.sh I(rs1!), rs2
//  - p.sb I(rs1!), rs2
//
//  - p.sw rs3(rs1), rs2
//  - p.sh rs3(rs1), rs2
//  - p.sb rs3(rs1), rs2
//
//  - p.sw rs3(rs1!), rs2
//  - p.sh rs3(rs1!), rs2
//  - p.sb rs3(rs1!), rs2
//
////////////////////////////////////////////////////////////////////////////////

#include "utils.h"
#include "string_lib.h"
#include "riscv_test.h"

void check_ld_postin_imm(void);
void check_ld_reg(void);
void check_ld_postin_reg(void);

void check_st_postin_imm(void);
void check_st_reg(void);
void check_st_postin_reg(void);

int main()
{
  volatile int * ptr_status;

  if(get_core_id() == 0) {
    test_init();

    check_ld_postin_imm();
    check_ld_reg();
    check_ld_postin_reg();

    check_st_postin_imm();
    check_st_reg();
    check_st_postin_reg();

    test_report();

    ptr_status = (int*)(0x10001000+get_core_id()*4*2);
    ptr_status[1] = test_get_num_errors();

    if (test_get_num_errors() == 0)
    {
        ptr_status[0] = 0xABBAABBA;
    }
    else
    {
        ptr_status[0] = 0xDEADBEEF;
    }
  }

  return 0;
}

#include "stimuli.h"


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// LOADS
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


//================================================================
// Check l* rd, I(rs1!)
//================================================================
void check_ld_postin_imm(void) {
  unsigned int i;

  //-----------------------------------------------------------------
  // Load word with postincrement in imm
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_ldpre_imm_c)/8); i++) {
    unsigned int pointer = (unsigned int)&g_ldpre_imm_c[2*i];
    unsigned int ld = 0;
    unsigned int ld1 = 0;

    asm volatile ("p.lw  %[ld],  0x4(%[c]!)\n"
                  "p.lw  %[ld1], 0x4(%[c]!)\n"
                  : [ld] "+r" (ld), [ld1] "+r" (ld1), [c] "+r" (pointer));

    test_check("p.lw  rd, I(rs1!), load nr. 1", ld,            g_ldpre_imm_c[2*i]);
    test_check("p.lw  rd, I(rs1!), load nr. 2", ld1,           g_ldpre_imm_c[2*i+1]);
    test_check("p.lw  rd, I(rs1!), addr", pointer, (uint32_t) &g_ldpre_imm_c[2*i+2]);
  }

  //-----------------------------------------------------------------
  // Load half word (zero-extend) with postincrement in imm
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_ldpre_imm_c)/8); i++) {
    unsigned int pointer = (unsigned int)&g_ldpre_imm_c[2*i];
    unsigned int ld = 0;
    unsigned int ld1 = 0;

    asm volatile ("p.lhu %[ld],  0x4(%[c]!)\n"
                  "p.lhu %[ld1], 0x4(%[c]!)\n"
                  : [ld] "+r" (ld), [ld1] "+r" (ld1), [c] "+r" (pointer));

    test_check("p.lhu rd, I(rs1!), load nr. 1", ld,            g_ldpre_imm_c[2*i]   & 0xFFFF);
    test_check("p.lhu rd, I(rs1!), load nr. 2", ld1,           g_ldpre_imm_c[2*i+1] & 0xFFFF);
    test_check("p.lhu rd, I(rs1!), addr", pointer, (uint32_t) &g_ldpre_imm_c[2*i+2]);
  }

  //-----------------------------------------------------------------
  // Load half word (sign-extend) with postincrement in imm
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_ldpre_imm_c)/8); i++) {
    unsigned int pointer = (unsigned int)&g_ldpre_imm_c[2*i];
    short ld = 0;
    short ld1 = 0;

    asm volatile ("p.lh  %[ld],  0x4(%[c]!)\n"
                  "p.lh  %[ld1], 0x4(%[c]!)\n"
                  : [ld] "+r" (ld), [ld1] "+r" (ld1), [c] "+r" (pointer));

    test_check("p.lh  rd, I(rs1!), load nr. 1", ld,     (short)(g_ldpre_imm_c[2*i]));
    test_check("p.lh  rd, I(rs1!), load nr. 2", ld1,    (short)(g_ldpre_imm_c[2*i+1]));
    test_check("p.lh  rd, I(rs1!), addr", pointer,   (uint32_t)&g_ldpre_imm_c[2*i+2]);
  }

  //-----------------------------------------------------------------
  // Load byte (zero-extend) with postincrement in imm
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_ldpre_imm_c)/8); i++) {
    unsigned int pointer = (unsigned int)&g_ldpre_imm_c[2*i];
    unsigned char ld = 0;
    unsigned char ld1 = 0;

    asm volatile ("p.lbu %[ld], 0x4(%[c]!)\n"
                  "p.lbu %[ld1], 0x4(%[c]!)\n"
                  : [ld] "+r" (ld), [ld1] "+r" (ld1), [c] "+r" (pointer));

    test_check("p.lbu rd, I(rs1!), load nr. 1", ld,             g_ldpre_imm_c[2*i]   & 0xFF);
    test_check("p.lbu rd, I(rs1!), load nr. 2", ld1,            g_ldpre_imm_c[2*i+1] & 0xFF);
    test_check("p.lbu rd, I(rs1!), addr", pointer,   (uint32_t)&g_ldpre_imm_c[2*i+2]);
  }

  //-----------------------------------------------------------------
  // Load byte (sign-extend) with postincrement in imm
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_ldpre_imm_c)/8); i++) {
    unsigned int pointer = (unsigned int)&g_ldpre_imm_c[2*i];
    char ld = 0;
    char ld1 = 0;

    asm volatile ("p.lb  %[ld],  0x4(%[c]!)\n"
                  "p.lb  %[ld1], 0x4(%[c]!)\n"
                  : [ld] "+r" (ld), [ld1] "+r" (ld1), [c] "+r" (pointer));

    test_check("p.lb  rd, I(rs1!), load nr. 1", ld,      (char)(g_ldpre_imm_c[2*i]));
    test_check("p.lb  rd, I(rs1!), load nr. 2", ld1,     (char)(g_ldpre_imm_c[2*i+1]));
    test_check("p.lb  rd, I(rs1!), addr", pointer,   (uint32_t)&g_ldpre_imm_c[2*i+2]);
  }
}

//================================================================
// Check p.l* rd, rs2(rs1)
//================================================================
void check_ld_reg(void) {
  unsigned int i;

  //-----------------------------------------------------------------
  // Load word with offset in registers
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_ld_reg_c)/8); i++) {
    unsigned int pointer = (unsigned int)&g_ld_reg_c[2*i];
    unsigned int ld = 0;
    unsigned int ld1 = 0;

    asm volatile ("p.lw  %[ld], x0(%[c])\n"
                  "p.lw  %[ld1], %[off](%[c])\n"
                  : [ld] "+r" (ld), [ld1] "+r" (ld1)
                  : [c] "r" (pointer), [off] "r" (g_ld_reg_off[i]));

    test_check("p.lw  rd, rs2(rs1), load nr. 1", ld,  g_ld_reg_c[2*i]);
    test_check("p.lw  rd, rs2(rs1), load nr. 2", ld1, g_ld_reg_c[2*i+1]);
  }

  //-----------------------------------------------------------------
  // Load half word (zero-extended) with offset in registers
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_ld_reg_c)/8); i++) {
    unsigned int pointer = (unsigned int)&g_ld_reg_c[2*i];
    unsigned short ld = 0;
    unsigned short ld1 = 0;

    asm volatile ("p.lhu %[ld], x0(%[c])\n"
                  "p.lhu %[ld1], %[off](%[c])\n"
                  : [ld] "+r" (ld), [ld1] "+r" (ld1)
                  : [c] "r" (pointer), [off] "r" (g_ld_reg_off[i]));

    test_check("p.lhu rd, rs2(rs1), load nr. 1", ld,  g_ld_reg_c[2*i]   & 0xFFFF);
    test_check("p.lhu rd, rs2(rs1), load nr. 2", ld1, g_ld_reg_c[2*i+1] & 0xFFFF);
  }

  //-----------------------------------------------------------------
  // Load half word (sign-extended) with offset in registers
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_ld_reg_c)/8); i++) {
    unsigned int pointer = (unsigned int)&g_ld_reg_c[2*i];
    short ld = 0;
    short ld1 = 0;

    asm volatile ("p.lh  %[ld], x0(%[c])\n"
                  "p.lh  %[ld1], %[off](%[c])\n"
                  : [ld] "+r" (ld), [ld1] "+r" (ld1)
                  : [c] "r" (pointer), [off] "r" (g_ld_reg_off[i]));

    test_check("p.lh  rd, rs2(rs1), load nr. 1", ld,  (short)(g_ld_reg_c[2*i]));
    test_check("p.lh  rd, rs2(rs1), load nr. 2", ld1, (short)(g_ld_reg_c[2*i+1]));
  }

  //-----------------------------------------------------------------
  // Load byte (zero-extended) with offset in registers
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_ld_reg_c)/8); i++) {
    unsigned int pointer = (unsigned int)&g_ld_reg_c[2*i];
    unsigned char ld = 0;
    unsigned char ld1 = 0;

    asm volatile ("p.lbu %[ld], x0(%[c])\n"
                  "p.lbu %[ld1], %[off](%[c])\n"
                  : [ld] "+r" (ld), [ld1] "+r" (ld1)
                  : [c] "r" (pointer), [off] "r" (g_ld_reg_off[i]));

    test_check("p.lbu rd, rs2(rs1), load nr. 1", ld,  g_ld_reg_c[2*i]   & 0xFF);
    test_check("p.lbu rd, rs2(rs1), load nr. 2", ld1, g_ld_reg_c[2*i+1] & 0xFF);
  }

  //-----------------------------------------------------------------
  // Load byte (sign-extended) with offset in registers
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_ld_reg_c)/8); i++) {
    unsigned int pointer = (unsigned int)&g_ld_reg_c[2*i];
    char ld = 0;
    char ld1 = 0;

    asm volatile ("p.lb  %[ld], x0(%[c])\n"
                  "p.lb  %[ld1], %[off](%[c])\n"
                  : [ld] "+r" (ld), [ld1] "+r" (ld1)
                  : [c] "r" (pointer), [off] "r" (g_ld_reg_off[i]));

    test_check("p.lb  rd, rs2(rs1), load nr. 1", ld,  (char)(g_ld_reg_c[2*i]));
    test_check("p.lb  rd, rs2(rs1), load nr. 2", ld1, (char)(g_ld_reg_c[2*i+1]));
  }
}

//================================================================
// Check p.l* rd, rs2(rs1!)
//================================================================
void check_ld_postin_reg(void) {
  unsigned int i;

  //-----------------------------------------------------------------
  // Load word with postincrement in reg
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_ldpre_reg_c)/8); i++) {
    unsigned int pointer = (unsigned int)&g_ldpre_reg_c[2*i];
    unsigned int ld = 0;
    unsigned int ld1 = 0;

    asm volatile ("p.lw  %[ld],  %[off](%[c]!)\n"
                  "p.lw  %[ld1], %[off](%[c]!)\n"
                  : [ld] "+r" (ld), [ld1] "+r" (ld1), [c] "+r" (pointer)
                  : [off] "r" (g_ldpre_reg_off[i]));

    test_check("p.lw  rd, rs2(rs1!), load nr. 1", ld,                 g_ldpre_reg_c[2*i]);
    test_check("p.lw  rd, rs2(rs1!), load nr. 2", ld1,                g_ldpre_reg_c[2*i+1]);
    test_check("p.lw  rd, rs2(rs1!), addr",       pointer, (uint32_t)&g_ldpre_reg_c[2*i+2]);
  }

  //-----------------------------------------------------------------
  // Load half word (zero-extended) with postincrement in reg
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_ldpre_reg_c)/8); i++) {
    unsigned int pointer = (unsigned int)&g_ldpre_reg_c[2*i];
    unsigned int ld = 0;
    unsigned int ld1 = 0;

    asm volatile ("p.lhu %[ld],  %[off](%[c]!)\n"
                  "p.lhu %[ld1], %[off](%[c]!)\n"
                  : [ld] "+r" (ld), [ld1] "+r" (ld1), [c] "+r" (pointer)
                  : [off] "r" (g_ldpre_reg_off[i]));

    test_check("p.lhu rd, rs2(rs1!), load nr. 1", ld,                 g_ldpre_reg_c[2*i]   & 0xFFFF);
    test_check("p.lhu rd, rs2(rs1!), load nr. 2", ld1,                g_ldpre_reg_c[2*i+1] & 0xFFFF);
    test_check("p.lhu rd, rs2(rs1!), addr",       pointer, (uint32_t)&g_ldpre_reg_c[2*i+2]);
  }

  //-----------------------------------------------------------------
  // Load half word (sign-extended) with postincrement in reg
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_ldpre_reg_c)/8); i++) {
    unsigned int pointer = (unsigned int)&g_ldpre_reg_c[2*i];
    short ld = 0;
    short ld1 = 0;

    asm volatile ("p.lh  %[ld],  %[off](%[c]!)\n"
                  "p.lh  %[ld1], %[off](%[c]!)\n"
                  : [ld] "+r" (ld), [ld1] "+r" (ld1), [c] "+r" (pointer)
                  : [off] "r" (g_ldpre_reg_off[i]));

    test_check("p.lh  rd, rs2(rs1!), load nr. 1", ld,         (short)(g_ldpre_reg_c[2*i]));
    test_check("p.lh  rd, rs2(rs1!), load nr. 2", ld1,        (short)(g_ldpre_reg_c[2*i+1]));
    test_check("p.lh  rd, rs2(rs1!), addr",       pointer, (uint32_t)&g_ldpre_reg_c[2*i+2]);
  }

  //-----------------------------------------------------------------
  // Load byte (zero-extended) with postincrement in reg
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_ldpre_reg_c)/8); i++) {
    unsigned int pointer = (unsigned int)&g_ldpre_reg_c[2*i];
    unsigned char ld = 0;
    unsigned char ld1 = 0;

    asm volatile ("p.lbu %[ld],  %[off](%[c]!)\n"
                  "p.lbu %[ld1], %[off](%[c]!)\n"
                  : [ld] "+r" (ld), [ld1] "+r" (ld1), [c] "+r" (pointer)
                  : [off] "r" (g_ldpre_reg_off[i]));

    test_check("p.lbu rd, rs2(rs1!), load nr. 1", ld,                 g_ldpre_reg_c[2*i]   & 0xFF);
    test_check("p.lbu rd, rs2(rs1!), load nr. 2", ld1,                g_ldpre_reg_c[2*i+1] & 0xFF);
    test_check("p.lbu rd, rs2(rs1!), addr",       pointer, (uint32_t)&g_ldpre_reg_c[2*i+2]);
  }

  //-----------------------------------------------------------------
  // Load byte (sign-extended) with postincrement in reg
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_ldpre_reg_c)/8); i++) {
    unsigned int pointer = (unsigned int)&g_ldpre_reg_c[2*i];
    char ld = 0;
    char ld1 = 0;

    asm volatile ("p.lb  %[ld],  %[off](%[c]!)\n"
                  "p.lb  %[ld1], %[off](%[c]!)\n"
                  : [ld] "+r" (ld), [ld1] "+r" (ld1), [c] "+r" (pointer)
                  : [off] "r" (g_ldpre_reg_off[i]));

    test_check("p.lb  rd, rs2(rs1!), load nr. 1", ld,          (char)(g_ldpre_reg_c[2*i]));
    test_check("p.lb  rd, rs2(rs1!), load nr. 2", ld1,         (char)(g_ldpre_reg_c[2*i+1]));
    test_check("p.lb  rd, rs2(rs1!), addr",       pointer, (uint32_t)&g_ldpre_reg_c[2*i+2]);
  }
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// STORES
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


//================================================================
// Check p.s* rd, I(rs1!)
//================================================================
void check_st_postin_imm(void) {
  unsigned int i;

  //-----------------------------------------------------------------
  // Store word with postincrement in imm
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_stpre_imm_st)/8); i++) {
    unsigned int pointer = (unsigned int)&g_stpre_imm_st[2*i];

    asm volatile ("p.sw %[c],  0x4(%[st]!)\n"
                  "p.sw %[c1], 0x4(%[st]!)\n"
                  : [c] "+r" (g_stpre_imm_c[2*i]), [c1] "+r" (g_stpre_imm_c[2*i+1]), [st] "+r" (pointer));

    test_check("p.sw rs1, I(rs2!), store nr. 1", g_stpre_imm_st[2*i],   g_stpre_imm_c[2*i]);
    test_check("p.sw rs1, I(rs2!), store nr. 2", g_stpre_imm_st[2*i+1], g_stpre_imm_c[2*i+1]);
    test_check("p.sw rs1, I(rs2!), addr",        pointer,    (uint32_t)&g_stpre_imm_st[2*i+2]);
  }

  //-----------------------------------------------------------------
  // Store half word with postincrement in imm
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_stpre_imm_st)/8); i++) {
    unsigned int pointer = (unsigned int)&g_stpre_imm_st[2*i];

    g_stpre_imm_st[2*i] = 0;
    g_stpre_imm_st[2*i+1] = 0;

    asm volatile ("p.sh %[c],  0x4(%[st]!)\n"
                  "p.sh %[c1], 0x4(%[st]!)\n"
                  : [c] "+r" (g_stpre_imm_c[2*i]), [c1] "+r" (g_stpre_imm_c[2*i+1]), [st] "+r" (pointer));

    test_check("p.sh rs1, I(rs2!), store nr. 1", g_stpre_imm_st[2*i],   g_stpre_imm_c[2*i]   & 0xFFFF);
    test_check("p.sh rs1, I(rs2!), store nr. 2", g_stpre_imm_st[2*i+1], g_stpre_imm_c[2*i+1] & 0xFFFF);
    test_check("p.sh rs1, I(rs2!), addr",        pointer,    (uint32_t)&g_stpre_imm_st[2*i+2]);
  }

  //-----------------------------------------------------------------
  // Store half word with postincrement in imm
  // check if packed half words work
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_stpre_imm_st)/8); i++) {
    unsigned int pointer = ((unsigned int)&g_stpre_imm_st[2*i]);

    g_stpre_imm_st[2*i] = 0;

    asm volatile ("p.sh %[c],  0x2(%[st]!)\n"
                  "p.sh %[c1], 0x2(%[st]!)\n"
                  : [c] "+r" (g_stpre_imm_c[2*i]), [c1] "+r" (g_stpre_imm_c[2*i+1]), [st] "+r" (pointer));

    test_check("p.sh rs1, I(rs2!)", g_stpre_imm_st[2*i], (g_stpre_imm_c[2*i] & 0xFFFF) | ((g_stpre_imm_c[2*i+1] & 0xFFFF) << 16));
    test_check("p.sh rs1, I(rs2!), addr", pointer, ((unsigned int)&g_stpre_imm_st[2*i]) + 0x4);
  }

  //-----------------------------------------------------------------
  // Store byte with postincrement in imm
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_stpre_imm_st)/8); i++) {
    unsigned int pointer = (unsigned int)&g_stpre_imm_st[2*i];

    g_stpre_imm_st[2*i] = 0;
    g_stpre_imm_st[2*i+1] = 0;

    asm volatile ("p.sb %[c],  0x4(%[st]!)\n"
                  "p.sb %[c1], 0x4(%[st]!)\n"
                  : [c] "+r" (g_stpre_imm_c[2*i]), [c1] "+r" (g_stpre_imm_c[2*i+1]), [st] "+r" (pointer));

    test_check("p.sb rs1, I(rs2!), store nr. 1", g_stpre_imm_st[2*i],   g_stpre_imm_c[2*i]   & 0xFF);
    test_check("p.sb rs1, I(rs2!), store nr. 2", g_stpre_imm_st[2*i+1], g_stpre_imm_c[2*i+1] & 0xFF);
    test_check("p.sb rs1, I(rs2!), addr",        pointer,    (uint32_t)&g_stpre_imm_st[2*i+2]);
  }

  //-----------------------------------------------------------------
  // Store byte with postincrement in imm
  // check if packed bytes work
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_stpre_imm_st)/8); i++) {
    unsigned int pointer = ((unsigned int)&g_stpre_imm_st[2*i]);

    g_stpre_imm_st[2*i] = 0;

    asm volatile ("p.sb %[c],  0x1(%[st]!)\n"
                  "p.sb %[c1], 0x1(%[st]!)\n"
                  "p.sb %[c],  0x1(%[st]!)\n"
                  "p.sb %[c1], 0x1(%[st]!)\n"
                  : [c] "+r" (g_stpre_imm_c[2*i]), [c1] "+r" (g_stpre_imm_c[2*i+1]), [st] "+r" (pointer));

    test_check("p.sb rs1, I(rs2!)", g_stpre_imm_st[2*i], (g_stpre_imm_c[2*i] & 0xFF) | ((g_stpre_imm_c[2*i+1] & 0xFF) << 8) | ((g_stpre_imm_c[2*i] & 0xFF) << 16) | ((g_stpre_imm_c[2*i+1] & 0xFF) << 24));
    test_check("p.sb rs1, I(rs2!), addr", pointer, ((unsigned int)&g_stpre_imm_st[2*i]) + 0x4);
  }
}

//================================================================
// Check p.s* rs3(rs2), rs1
//================================================================
void check_st_reg(void) {
  unsigned int i;

  //-----------------------------------------------------------------
  // Store word with offset in registers
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_st_reg_st)/8); i++) {
    unsigned int pointer = (unsigned int)&g_st_reg_st[2*i];

    g_st_reg_st[2*i] = 0;
    g_st_reg_st[2*i+1] = 0;

    asm volatile ("p.sw %[c],      x0(%[st])\n"
                  "p.sw %[c1], %[off](%[st])\n"
                  : [st]  "+r"  (pointer),
                    [c]   "+r" (g_st_reg_c[2*i]),
                    [c1]  "+r" (g_st_reg_c[2*i+1]),
                    [off] "+r" (g_st_reg_off[i]));

    test_check("p.sw rs3(rs1), rs2, store nr. 1", g_st_reg_st[2*i],   g_st_reg_c[2*i]);
    test_check("p.sw rs3(rs1), rs2, store nr. 2", g_st_reg_st[2*i+1], g_st_reg_c[2*i+1]);
  }

  //-----------------------------------------------------------------
  // Store half word with offset in register
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_st_reg_st)/8); i++) {
    unsigned int pointer = (unsigned int)&g_st_reg_st[2*i];

    g_st_reg_st[2*i] = 0;
    g_st_reg_st[2*i+1] = 0;

    asm volatile ("p.sh %[c],      x0(%[st])\n"
                  "p.sh %[c1], %[off](%[st])\n"
                  : : [st]  "r"  (pointer),
                      [c]   "r" (g_st_reg_c[2*i]),
                      [c1]  "r" (g_st_reg_c[2*i+1]),
                      [off] "r" (g_st_reg_off[i]));

    test_check("p.sh rs3(rs1), rs2, store nr. 1", g_st_reg_st[2*i],   g_st_reg_c[2*i]   & 0xFFFF);
    test_check("p.sh rs3(rs1), rs2, store nr. 2", g_st_reg_st[2*i+1], g_st_reg_c[2*i+1] & 0xFFFF);
  }

  //-----------------------------------------------------------------
  // Store byte with offset in register
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_st_reg_st)/8); i++) {
    unsigned int pointer = (unsigned int)&g_st_reg_st[2*i];

    g_st_reg_st[2*i] = 0;
    g_st_reg_st[2*i+1] = 0;

    asm volatile ("p.sb %[c],      x0(%[st])\n"
                  "p.sb %[c1], %[off](%[st])\n"
                  : : [st]  "r" (pointer),
                      [c]   "r" (g_st_reg_c[2*i]),
                      [c1]  "r" (g_st_reg_c[2*i+1]),
                      [off] "r" (g_st_reg_off[i]));

    test_check("p.sb rs3(rs1), rs2, store nr. 1", g_st_reg_st[2*i],   g_st_reg_c[2*i]   & 0xFF);
    test_check("p.sb rs3(rs1), rs2, store nr. 2", g_st_reg_st[2*i+1], g_st_reg_c[2*i+1] & 0xFF);
  }
}

//================================================================
// Check l.s* rs3(rs2!), rs1
//================================================================
void check_st_postin_reg(void) {
  unsigned int i;

  //-----------------------------------------------------------------
  // Store word with offset in registers, post-increment
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_st_reg_st)/8); i++) {
    unsigned int pointer = (unsigned int)&g_st_reg_st[2*i];

    g_st_reg_st[2*i] = 0;
    g_st_reg_st[2*i+1] = 0;

    asm volatile ("p.sw %[c],  %[off](%[st]!)\n"
                  "p.sw %[c1],     x0(%[st]!)\n"
                  : [st]  "+&r" (pointer)
                  : [c]   "r"   (g_st_reg_c[2*i]),
                    [c1]  "r"   (g_st_reg_c[2*i+1]),
                    [off] "r"   (g_st_reg_off[i]));

    test_check("p.sw rs3(rs1!), rs2, store nr. 1", g_st_reg_st[2*i],              g_st_reg_c[2*i]);
    test_check("p.sw rs3(rs1!), rs2, store nr. 2", g_st_reg_st[2*i+1],            g_st_reg_c[2*i+1]);
    test_check("p.sw rs3(rs1!), rs2, addr",        pointer,            (uint32_t)&g_st_reg_st[2*i+1]);
  }

  //-----------------------------------------------------------------
  // Store half word with offset in register, post-increment
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_st_reg_st)/8); i++) {
    unsigned int pointer = (unsigned int)&g_st_reg_st[2*i];

    g_st_reg_st[2*i] = 0;
    g_st_reg_st[2*i+1] = 0;

    asm volatile ("nop;" "p.sh %[c],  %[off](%[st]!)\n"
                  "p.sh %[c1],     x0(%[st]!)\n"
                  : [st]  "+&r" (pointer)
                  : [c]   "r"   (g_st_reg_c[2*i]),
                    [c1]  "r"   (g_st_reg_c[2*i+1]),
                    [off] "r"   (g_st_reg_off[i]));

    test_check("p.sh rs3(rs1!), rs2, store nr. 1", g_st_reg_st[2*i],   g_st_reg_c[2*i]   & 0xFFFF);
    test_check("p.sh rs3(rs1!), rs2, store nr. 2", g_st_reg_st[2*i+1], g_st_reg_c[2*i+1] & 0xFFFF);
    test_check("p.sw rs3(rs1!), rs2, addr",        pointer, (uint32_t)&g_st_reg_st[2*i+1]);
  }

  //-----------------------------------------------------------------
  // Store byte with offset in register, post-increment
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_st_reg_st)/8); i++) {
    unsigned int pointer = (unsigned int)&g_st_reg_st[2*i];

    g_st_reg_st[2*i] = 0;
    g_st_reg_st[2*i+1] = 0;

    asm volatile ("nop;" "p.sb %[c],  %[off](%[st]!)\n"
                  "p.sb %[c1],     x0(%[st]!)\n"
                  : [st]  "+&r" (pointer)
                  : [c]   "r"   (g_st_reg_c[2*i]),
                    [c1]  "r"   (g_st_reg_c[2*i+1]),
                    [off] "r"   (g_st_reg_off[i]));

    test_check("p.sb rs3(rs1!), rs2, store nr. 1", g_st_reg_st[2*i],              g_st_reg_c[2*i]   & 0xFF);
    test_check("p.sb rs3(rs1!), rs2, store nr. 2", g_st_reg_st[2*i+1],            g_st_reg_c[2*i+1] & 0xFF);
    test_check("p.sw rs3(rs1!), rs2, addr",        pointer,            (uint32_t)&g_st_reg_st[2*i+1]);
  }
}

