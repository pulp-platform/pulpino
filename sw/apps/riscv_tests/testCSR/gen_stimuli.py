#!/usr/bin/env python

import sys
import random

# Copyright 2017 ETH Zurich and University of Bologna.
# Copyright and related rights are licensed under the Solderpad Hardware
# License, Version 0.51 (the License); you may not use this file except in
# compliance with the License.  You may obtain a copy of the License at
# http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
# or agreed to in writing, software, hardware and materials distributed under
# this License is distributed on an AS IS BASIS, WITHOUT WARRANTIES OR
# CONDITIONS OF ANY KIND, either express or implied. See the License for the
# specific language governing permissions and limitations under the License.

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
instr_32_r('g_mstatus',  0x00, lambda a, p: (a & 0x88 | 0x1800))
instr_32_r('g_mimpid',   0x00, lambda a, p: 0x0000)
instr_32_r('g_mhartid',  0x00, lambda a, p: 0x0000) # for core 0 in cluster 0

