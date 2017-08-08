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


N = 20
f = open('testALUExt_stimuli.h', 'w')


################################################################################
# instructions with two read registers
################################################################################

def instr_rr_base(prefix, func, mina, maxa, minb, maxb):
    global f

    ops_a    = []
    ops_b    = []
    exp_res  = []

    for i in range(N):
        a = random.randint(mina, maxa)
        b = random.randint(minb, maxb)

        r = func(a, b)

        ops_a.append(a)
        ops_b.append(b)
        exp_res.append(r)

    write_hex32_arr(f, '%s_a'   % prefix, ops_a)
    write_hex32_arr(f, '%s_b'   % prefix, ops_b)
    write_hex32_arr(f, '%s_exp' % prefix, exp_res)

def instr_rr(prefix, func):
    instr_rr_base(prefix, func, -2**31, 2**31-1, -2**31, 2**31-1)

def instr_rr_u(prefix, func):
    instr_rr_base(prefix, func, 0, 2**32-1, 0, 2**32-1)

def instr_rr_u_ror(prefix, func):
    instr_rr_base(prefix, func, 0, 2**32-1, 0, 31)


################################################################################
# Instructions with one read register
################################################################################

def instr_32_r(prefix, func):
    global f

    ops_a    = []
    exp_res  = []

    for i in range(N):
        a = random.randint(-2**31, 2**31-1)

        r = func(a)

        ops_a.append(a)
        exp_res.append(r)

    write_hex32_arr(f, '%s_a'   % prefix, ops_a)
    write_hex32_arr(f, '%s_exp' % prefix, exp_res)


################################################################################
# generate testdata for l.avg and l.avgu
################################################################################

##def avg_32(a, b):
##    r = ((a + b) & 0xFFFFFFFF) >> 1
##    if(r >> 30):
##        r = r | 0x80000000;
##    return r
##
##def avgu_32(a, b):
##    r = ((a + b) & 0xFFFFFFFF) >> 1
##    return r
##
##instr_rr  ('g_avg',  avg_32)
##instr_rr_u('g_avgu', avgu_32)

################################################################################
# generate testdata for l.ror
################################################################################

def ror(a,b):
    r = a >> b
    r |= a << (32 - b)
    r &= 0xFFFFFFFF
    return r

instr_rr_u_ror('g_ror',  ror)


################################################################################
# generate testdata for l.min and l.minu
################################################################################

instr_rr  ('g_min',  lambda a, b: (b if (a > b) else a))
instr_rr_u('g_minu', lambda a, b: (b if (a > b) else a))

################################################################################
# generate testdata for l.max and l.maxu
################################################################################

instr_rr  ('g_max',  lambda a, b: (a if (a > b) else b))
instr_rr_u('g_maxu', lambda a, b: (a if (a > b) else b))

################################################################################
# generate testdata for l.abs
################################################################################

instr_rr('g_abs',  lambda a, b: (-a if (a < 0) else a))

################################################################################
# generate testdata for lv.ext
################################################################################

def sext_16(a):
    r = a & 0xFFFF
    if(r & 0x8000):
        r = r | 0xFFFF0000
    return r

def zext_16(a):
    return a & 0xFFFF

def sext_8(a):
    r = a & 0xFF
    if(r & 0x80):
        r = r | 0xFFFFFF00
    return r

def zext_8(a):
    return a & 0xFF


instr_32_r('g_ext_hs', sext_16)
instr_32_r('g_ext_hz', zext_16)
instr_32_r('g_ext_bs', sext_8)
instr_32_r('g_ext_bz', zext_8)