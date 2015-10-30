#!/usr/bin/env python-2.5

import sys
import random

def toint(val, nbits):
    return (val + (1 << nbits)) % (1 << nbits)

def tohex32(val):
    return hex((val + (1 << 32)) % (1 << 32))

def tohex32_hi(val):
    val = val >> 32
    return hex((val + (1 << 32)) % (1 << 32))

def write_uint32_arr(f, name, arr):
    f.write('unsigned int %s[] = {\n' % name)
    for v in arr:
        f.write('0x%08X,\n' % (v & 0xFFFFFFFF))
    f.write('};\n\n')
    return

def write_uint32_hi_arr(f, name, arr):
    f.write('unsigned int %s[] = {\n' % name)
    for v in arr:
        f.write('0x%08X,\n' % ((v >> 32) & 0xFFFFFFFF))
    f.write('};\n\n')
    return

def write_int32_arr(f, name, arr):
    f.write('int %s[] = {\n' % name)
    for v in arr:
        f.write('%s,\n' % (tohex32(v)))
    f.write('};\n\n')
    return

def write_int32_hi_arr(f, name, arr):
    f.write('int %s[] = {\n' % name)
    for v in arr:
        f.write('%s,\n' % (tohex32_hi(v)))
    f.write('};\n\n')
    return

def write_stimuli(f, prefix, ops_a, ops_b, res):
    act = []
    for v in res:
        act.append(0)

    write_uint32_arr(f, 'g_%sop_a' % prefix, ops_a)
    write_uint32_arr(f, 'g_%sop_b' % prefix, ops_b)
    write_uint32_arr(f, 'g_%sact' % prefix, act)
    write_uint32_arr(f, 'g_%sexp' % prefix, res)

    return

f = open('testPrePost_stimuli.h', 'w')

################################################################################
# generate data for load word with preincrement in immediate
################################################################################
ops_c   = []

for i in range(0,10):
    op_c = random.randint(-2**31, 2**31-1)
    op_c1 = random.randint(-2**31, 2**31-1)

    ops_c.append(op_c)
    ops_c.append(op_c1)

write_uint32_arr(f, 'g_ldpre_imm_c', ops_c)

################################################################################
# generate data for load word with offset in register
################################################################################
ops_off = []
ops_c   = []

for i in range(0,10):
    op_off = 4
    op_c = random.randint(-2**31, 2**31-1)
    op_c1 = random.randint(-2**31, 2**31-1)

    ops_off.append(op_off)
    ops_c.append(op_c)
    ops_c.append(op_c1)

write_uint32_arr(f, 'g_ld_reg_c', ops_c)
write_uint32_arr(f, 'g_ld_reg_off', ops_off)

################################################################################
# generate data for load word with preincrement in reg
################################################################################
ops_off = []
ops_c   = []

for i in range(0,10):
    op_off = 4
    op_c = random.randint(-2**31, 2**31-1)
    op_c1 = random.randint(-2**31, 2**31-1)

    ops_off.append(op_off)
    ops_c.append(op_c)
    ops_c.append(op_c1)

write_uint32_arr(f, 'g_ldpre_reg_c', ops_c)
write_uint32_arr(f, 'g_ldpre_reg_off', ops_off)

################################################################################
# generate data for store word with preincrement in immediate
################################################################################
ops_st  = []
ops_c   = []

for i in range(0,10):
    op_c = random.randint(-2**31, 2**31-1)
    op_c1 = random.randint(-2**31, 2**31-1)

    ops_c.append(op_c)
    ops_c.append(op_c1)

    ops_st.append(0)
    ops_st.append(0)

write_uint32_arr(f, 'g_stpre_imm_c', ops_c)
write_uint32_arr(f, 'g_stpre_imm_st', ops_st)

################################################################################
# generate data for store word with offset in register
################################################################################
ops_st  = []
ops_off = []
ops_c   = []

for i in range(0,10):
    op_off = 4
    op_c = random.randint(-2**31, 2**31-1)
    op_c1 = random.randint(-2**31, 2**31-1)

    ops_off.append(op_off)
    ops_c.append(op_c)
    ops_c.append(op_c)
    ops_c.append(op_c1)

    ops_st.append(0)
    ops_st.append(0)

write_uint32_arr(f, 'g_st_reg_c', ops_c)
write_uint32_arr(f, 'g_st_reg_off', ops_off)
write_uint32_arr(f, 'g_st_reg_st', ops_st)

