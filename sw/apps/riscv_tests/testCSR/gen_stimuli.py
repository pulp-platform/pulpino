#!/usr/bin/env python2

import sys
import random


def write_hex32_arr(f, name, arr):
    f.write('unsigned int %s[] = {\n' % name)
    for v in arr:
        f.write('0x%08X,\n' % (v & 0xFFFFFFFF))
    f.write('};\n\n')
    return


N = 15
f = open('testCSR_stimuli.h', 'w')


################################################################################
# Instructions with one read register
################################################################################

def instr_32_r(prefix, initial, func):
    global f

    ops_a    = []
    exp_res  = []
    prev     = initial

    for i in range(N):
        a = random.randint(-2**31, 2**31-1)

        r = func(a, prev)
        prev = r

        ops_a.append(a)
        exp_res.append(r)

    write_hex32_arr(f, '%s_a'   % prefix, ops_a)
    write_hex32_arr(f, '%s_exp' % prefix, exp_res)


################################################################################
# generate testdata
################################################################################

instr_32_r('g_csrrw', 0x00, lambda a, p: a)
instr_32_r('g_csrrs', 0x00, lambda a, p: (p | a))
instr_32_r('g_csrrc', 0xffffffff, lambda a, p: (p & (~a)))

instr_32_r('g_mepc',     0x00, lambda a, p: a)
instr_32_r('g_mstatus',  0x00, lambda a, p: (a & 0x01 | 0x6))
instr_32_r('g_mcpuid',   0x00, lambda a, p: 0x0100)
instr_32_r('g_mimpid',   0x00, lambda a, p: 0x8000)
instr_32_r('g_mhartid',  0x00, lambda a, p: 0x0000) # for core 0 in cluster 0

