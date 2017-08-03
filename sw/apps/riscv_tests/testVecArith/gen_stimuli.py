#!/usr/bin/env python

import sys
import random
import argparse

# Copyright 2017 ETH Zurich and University of Bologna.
# Copyright and related rights are licensed under the Solderpad Hardware
# License, Version 0.51 (the License); you may not use this file except in
# compliance with the License.  You may obtain a copy of the License at
# http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
# or agreed to in writing, software, hardware and materials distributed under
# this License is distributed on an AS IS BASIS, WITHOUT WARRANTIES OR
# CONDITIONS OF ANY KIND, either express or implied. See the License for the
# specific language governing permissions and limitations under the License.

parser = argparse.ArgumentParser(description='Generate stimuli')

parser.add_argument("--riscv", dest="riscv", default=False, action="store_true", help="Generate for riscv")

args = parser.parse_args()

if args.riscv: sciImm = 11
else: sciImm = 111

def write_hex8_arr(f, name, arr):
    f.write('unsigned int %s[] = {\n' % name)
    for v in arr:
        f.write('0x%02X%02X%02X%02X,\n' % (v[0] & 0xFF, v[1] & 0xFF, v[2] & 0xFF, v[3] & 0xFF))
    f.write('};\n\n')
    return

def write_hex16_arr(f, name, arr):
    f.write('unsigned int %s[] = {\n' % name)
    for v in arr:
        f.write('0x%04X%04X,\n' % (v[0] & 0xFFFF, v[1] & 0xFFFF))
    f.write('};\n\n')
    return

def write_hex32_arr(f, name, arr):
    f.write('unsigned int %s[] = {\n' % name)
    for v in arr:
        f.write('0x%08X,\n' % (v & 0xFFFFFFFF))
    f.write('};\n\n')
    return

################################################################################
# instructions with three registers
################################################################################

def vec_16_rrr_base(f, prefix, func, mina, maxa, minb, maxb, mind, maxd):
    
    ops_a    = []
    ops_b    = []
    ops_d    = []
    exp_res  = []
    
    for i in range(0,10):
        op_a  = []
        op_b  = []
        op_d  = []
        res   = []
    
        for j in range(0,2):
            a = random.randint(mina, maxa)
            b = random.randint(minb, maxb)
            d = random.randint(mind, maxd)
    
            r = func(a, b, d)
    
            op_a.append(a)
            op_b.append(b)
            op_d.append(d)
            res.append(r)

        ops_a.append(op_a)
        ops_b.append(op_b)
        ops_d.append(op_d)
        exp_res.append(res)
    
    write_hex16_arr(f, '%s_a'   % prefix, ops_a)
    write_hex16_arr(f, '%s_b'   % prefix, ops_b)
    write_hex16_arr(f, '%s_d'   % prefix, ops_d)
    write_hex16_arr(f, '%s_exp' % prefix, exp_res)

def vec_16_rrr_sc_base(f, prefix, func, mina, maxa, minb, maxb, mind, maxd):
    
    ops_a    = []
    ops_b    = []
    ops_d    = []
    exp_res  = []
    
    for i in range(0,10):
        op_a  = []
        op_b  = []
        op_d  = []
        res   = []

        b = random.randint(minb, maxb)
    
        for j in range(0,2):
            a = random.randint(mina, maxa)
            d = random.randint(mind, maxd)
    
            r = func(a, b, d)
    
            op_a.append(a)
            op_d.append(d)
            res.append(r)

            if(j == 1):
                op_b.append(b)
            else:
                op_b.append(random.randint(minb, maxb))

        ops_a.append(op_a)
        ops_b.append(op_b)
        ops_d.append(op_d)
        exp_res.append(res)
    
    write_hex16_arr(f, '%s_a'   % prefix, ops_a)
    write_hex16_arr(f, '%s_b'   % prefix, ops_b)
    write_hex16_arr(f, '%s_d'   % prefix, ops_d)
    write_hex16_arr(f, '%s_exp' % prefix, exp_res)

def vec_16_rrr_sci_base(f, prefix, func, mina, maxa, imm, mind, maxd):
    
    ops_a    = []
    ops_d    = []
    exp_res  = []
    
    for i in range(0,10):
        op_a  = []
        op_d  = []
        res   = []

        for j in range(0,2):
            a = random.randint(mina, maxa)
            d = random.randint(mind, maxd)
    
            r = func(a, imm, d)
    
            op_a.append(a)
            op_d.append(d)
            res.append(r)

        ops_a.append(op_a)
        ops_d.append(op_d)
        exp_res.append(res)
    
    write_hex16_arr(f, '%s_a'   % prefix, ops_a)
    write_hex16_arr(f, '%s_d'   % prefix, ops_d)
    write_hex16_arr(f, '%s_exp' % prefix, exp_res)

def vec_8_rrr_base(f, prefix, func, mina, maxa, minb, maxb, mind, maxd):
    
    ops_a    = []
    ops_b    = []
    ops_d    = []
    exp_res  = []
    
    for i in range(0,10):
        op_a  = []
        op_b  = []
        op_d  = []
        res   = []
    
        for j in range(0,4):
            a = random.randint(mina, maxa)
            b = random.randint(minb, maxb)
            d = random.randint(mind, maxd)
    
            r = func(a, b, d)
    
            op_a.append(a)
            op_b.append(b)
            op_d.append(d)
            res.append(r)

        ops_a.append(op_a)
        ops_b.append(op_b)
        ops_d.append(op_d)
        exp_res.append(res)
    
    write_hex8_arr(f, '%s_a'   % prefix, ops_a)
    write_hex8_arr(f, '%s_b'   % prefix, ops_b)
    write_hex8_arr(f, '%s_d'   % prefix, ops_d)
    write_hex8_arr(f, '%s_exp' % prefix, exp_res)

def vec_8_rrr_sc_base(f, prefix, func, mina, maxa, minb, maxb, mind, maxd):
    
    ops_a    = []
    ops_b    = []
    ops_d    = []
    exp_res  = []
    
    for i in range(0,10):
        op_a  = []
        op_b  = []
        op_d  = []
        res   = []

        b = random.randint(minb, maxb)
    
        for j in range(0,4):
            a = random.randint(mina, maxa)
            d = random.randint(mind, maxd)
    
            r = func(a, b ,d)
    
            op_a.append(a)
            op_d.append(d)
            res.append(r)

            if(j == 4):
                op_b.append(b)
            else:
                op_b.append(random.randint(minb, maxb)) # just a random value, we don't care

        ops_a.append(op_a)
        ops_b.append(op_b)
        ops_d.append(op_d)
        exp_res.append(res)
    
    write_hex8_arr(f, '%s_a'   % prefix, ops_a)
    write_hex8_arr(f, '%s_b'   % prefix, ops_b)
    write_hex8_arr(f, '%s_d'   % prefix, ops_d)
    write_hex8_arr(f, '%s_exp' % prefix, exp_res)

def vec_8_rrr_sci_base(f, prefix, func, mina, maxa, imm, mind, maxd):
    
    ops_a    = []
    ops_d    = []
    exp_res  = []
    
    for i in range(0,10):
        op_a  = []
        op_d  = []
        res   = []

        for j in range(0,4):
            a = random.randint(mina, maxa)
            d = random.randint(mind, maxd)
    
            r = func(a, imm, d)
    
            op_a.append(a)
            op_d.append(d)
            res.append(r)

        ops_a.append(op_a)
        ops_d.append(op_d)
        exp_res.append(res)
    
    write_hex8_arr(f, '%s_a'   % prefix, ops_a)
    write_hex8_arr(f, '%s_d'   % prefix, ops_d)
    write_hex8_arr(f, '%s_exp' % prefix, exp_res)

def vec_16_rrr(f, prefix, func):
    vec_16_rrr_base(f, prefix, func, -2**15, 2**15-1, -2**15, 2**15-1, -2**15, 2**15-1)

def vec_16_rrr_sc(f, prefix, func):
    vec_16_rrr_sc_base(f, prefix, func, -2**15, 2**15-1, -2**15, 2**15-1, -2**15, 2**15-1)

def vec_16_rrr_sci(f, prefix, func, imm):
    vec_16_rrr_sci_base(f, prefix, func, -2**15, 2**15-1, imm, -2**15, 2**15-1)

def vec_8_rrr(f, prefix, func):
    vec_8_rrr_base(f, prefix, func, -2**7, 2**7-1, -2**7, 2**7-1, -2**7, 2**7-1)

def vec_8_rrr_sc(f, prefix, func):
    vec_8_rrr_sc_base(f, prefix, func, -2**7, 2**7-1, -2**7, 2**7-1, -2**7, 2**7-1)

def vec_8_rrr_sci(f, prefix, func, imm):
    vec_8_rrr_sci_base(f, prefix, func, -2**7, 2**7-1, imm, -2**7, 2**7-1)

################################################################################
# instructions with two read registers
################################################################################

def instr_rr_base(f, prefix, func, mina, maxa, minb, maxb):
    
    ops_a    = []
    ops_b    = []
    exp_res  = []
    
    for i in range(0,10):
        a = random.randint(mina, maxa)
        b = random.randint(minb, maxb)
    
        r = func(a, b)
    
        ops_a.append(a)
        ops_b.append(b)
        exp_res.append(r)
    
    write_hex32_arr(f, '%s_a'   % prefix, ops_a)
    write_hex32_arr(f, '%s_b'   % prefix, ops_b)
    write_hex32_arr(f, '%s_exp' % prefix, exp_res)

def vec_16_rr_base(f, prefix, func, mina, maxa, minb, maxb):
    
    ops_a    = []
    ops_b    = []
    exp_res  = []
    
    for i in range(0,10):
        op_a  = []
        op_b  = []
        res   = []
    
        for j in range(0,2):
            a = random.randint(mina, maxa)
            b = random.randint(minb, maxb)
    
            r = func(a, b)
    
            op_a.append(a)
            op_b.append(b)
            res.append(r)

        ops_a.append(op_a)
        ops_b.append(op_b)
        exp_res.append(res)
    
    write_hex16_arr(f, '%s_a'   % prefix, ops_a)
    write_hex16_arr(f, '%s_b'   % prefix, ops_b)
    write_hex16_arr(f, '%s_exp' % prefix, exp_res)

def vec_16_rr_sc_base(f, prefix, func, mina, maxa, minb, maxb):
    
    ops_a    = []
    ops_b    = []
    exp_res  = []
    
    for i in range(0,10):
        op_a  = []
        op_b  = []
        res   = []

        b = random.randint(minb, maxb)
    
        for j in range(0,2):
            a = random.randint(mina, maxa)
    
            r = func(a, b)
    
            op_a.append(a)
            res.append(r)

            if(j == 1):
                op_b.append(b)
            else:
                op_b.append(random.randint(minb, maxb))

        ops_a.append(op_a)
        ops_b.append(op_b)
        exp_res.append(res)
    
    write_hex16_arr(f, '%s_a'   % prefix, ops_a)
    write_hex16_arr(f, '%s_b'   % prefix, ops_b)
    write_hex16_arr(f, '%s_exp' % prefix, exp_res)

def vec_16_rr_sci_base(f, prefix, func, mina, maxa, imm):
    
    ops_a    = []
    exp_res  = []
    
    for i in range(0,10):
        op_a  = []
        res   = []

        for j in range(0,2):
            a = random.randint(mina, maxa)
    
            r = func(a, imm)
    
            op_a.append(a)
            res.append(r)

        ops_a.append(op_a)
        exp_res.append(res)
    
    write_hex16_arr(f, '%s_a'   % prefix, ops_a)
    write_hex16_arr(f, '%s_exp' % prefix, exp_res)

def vec_8_rr_base(f, prefix, func, mina, maxa, minb, maxb):
    
    ops_a    = []
    ops_b    = []
    exp_res  = []
    
    for i in range(0,10):
        op_a  = []
        op_b  = []
        res   = []
    
        for j in range(0,4):
            a = random.randint(mina, maxa)
            b = random.randint(minb, maxb)
    
            r = func(a, b)
    
            op_a.append(a)
            op_b.append(b)
            res.append(r)

        ops_a.append(op_a)
        ops_b.append(op_b)
        exp_res.append(res)
    
    write_hex8_arr(f, '%s_a'   % prefix, ops_a)
    write_hex8_arr(f, '%s_b'   % prefix, ops_b)
    write_hex8_arr(f, '%s_exp' % prefix, exp_res)

def vec_8_rr_sc_base(f, prefix, func, mina, maxa, minb, maxb):
    
    ops_a    = []
    ops_b    = []
    exp_res  = []
    
    for i in range(0,10):
        op_a  = []
        op_b  = []
        res   = []

        b = random.randint(minb, maxb)
    
        for j in range(0,4):
            a = random.randint(mina, maxa)
    
            r = func(a, b)
    
            op_a.append(a)
            res.append(r)

            if(j == 3):
                op_b.append(b)
            else:
                op_b.append(random.randint(minb, maxb)) # just a random value, we don't care

        ops_a.append(op_a)
        ops_b.append(op_b)
        exp_res.append(res)
    
    write_hex8_arr(f, '%s_a'   % prefix, ops_a)
    write_hex8_arr(f, '%s_b'   % prefix, ops_b)
    write_hex8_arr(f, '%s_exp' % prefix, exp_res)

def vec_8_rr_sci_base(f, prefix, func, mina, maxa, imm):
    
    ops_a    = []
    exp_res  = []
    
    for i in range(0,10):
        op_a  = []
        res   = []

        for j in range(0,4):
            a = random.randint(mina, maxa)
    
            r = func(a, imm)
    
            op_a.append(a)
            res.append(r)

        ops_a.append(op_a)
        exp_res.append(res)
    
    write_hex8_arr(f, '%s_a'   % prefix, ops_a)
    write_hex8_arr(f, '%s_exp' % prefix, exp_res)

def instr_rr(f, prefix, func):
    instr_rr_base(f, prefix, func, -2**31, 2**31-1, -2**31, 2**31-1)

def instr_rr_u(f, prefix, func):
    instr_rr_base(f, prefix, func, 0, 2**32-1, 0, 2**32-1)

def vec_16_rr(f, prefix, func):
    vec_16_rr_base(f, prefix, func, -2**15, 2**15-1, -2**15, 2**15-1)

def vec_16_rr_sc(f, prefix, func):
    vec_16_rr_sc_base(f, prefix, func, -2**15, 2**15-1, -2**15, 2**15-1)

def vec_16_rr_sci(f, prefix, func, imm):
    vec_16_rr_sci_base(f, prefix, func, -2**15, 2**15-1, imm)

def vec_8_rr(f, prefix, func):
    vec_8_rr_base(f, prefix, func, -2**7, 2**7-1, -2**7, 2**7-1)

def vec_8_rr_sc(f, prefix, func):
    vec_8_rr_sc_base(f, prefix, func, -2**7, 2**7-1, -2**7, 2**7-1)

def vec_8_rr_sci(f, prefix, func, imm):
    vec_8_rr_sci_base(f, prefix, func, -2**7, 2**7-1, imm)

def vec_16_rr_shift(f, prefix, func):
    vec_16_rr_base(f, prefix, func, -2**15, 2**15-1, 0, 15)

def vec_16_rr_shift_sc(f, prefix, func):
    vec_16_rr_sc_base(f, prefix, func, -2**15, 2**15-1, 0, 15)

def vec_16_rr_shift_sci(f, prefix, func, imm):
    vec_16_rr_sci_base(f, prefix, func, -2**15, 2**15-1, imm)

def vec_8_rr_shift(f, prefix, func):
    vec_8_rr_base(f, prefix, func, -2**7, 2**7-1, 0, 7)

def vec_8_rr_shift_sc(f, prefix, func):
    vec_8_rr_sc_base(f, prefix, func, -2**7, 2**7-1, 0, 7)

def vec_8_rr_shift_sci(f, prefix, func, imm):
    vec_8_rr_sci_base(f, prefix, func, -2**7, 2**7-1, imm)

################################################################################
# Instructions with one read register only
################################################################################
def vec_16_r_base(f, prefix, func, min, max):
    
    ops_a    = []
    exp_res  = []
    
    for i in range(0,10):
        op_a  = []
        res   = []
    
        for j in range(0,2):
            a = random.randint(min, max)
    
            r = func(a)
    
            op_a.append(a)
            res.append(r)

        ops_a.append(op_a)
        exp_res.append(res)
    
    write_hex16_arr(f, '%s_a'   % prefix, ops_a)
    write_hex16_arr(f, '%s_exp' % prefix, exp_res)

def vec_8_r_base(f, prefix, func, min, max):
    
    ops_a    = []
    exp_res  = []
    
    for i in range(0,10):
        op_a  = []
        res   = []
    
        for j in range(0,4):
            a = random.randint(min, max)
    
            r = func(a)
    
            op_a.append(a)
            res.append(r)

        ops_a.append(op_a)
        exp_res.append(res)
    
    write_hex8_arr(f, '%s_a'   % prefix, ops_a)
    write_hex8_arr(f, '%s_exp' % prefix, exp_res)

def vec_8_r(f, prefix, func):
    vec_8_r_base(f, prefix, func, -2**7, 2**7-1)

def vec_16_r(f, prefix, func):
    vec_16_r_base(f, prefix, func, -2**15, 2**15-1)

################################################################################
# Instructions with two read register and one immediate
################################################################################
def instr_32_rri(f, prefix, func, imm):
    
    ops_a    = []
    ops_b    = []
    exp_res  = []
    
    for i in range(0,10):
        a = random.randint(-2**31, 2**31-1)
        b = random.randint(-2**31, 2**31-1)

        r = func(a, b, imm)

        ops_a.append(a)
        ops_b.append(b)
        exp_res.append(r)
    
    write_hex32_arr(f, '%s_a'   % prefix, ops_a)
    write_hex32_arr(f, '%s_b'   % prefix, ops_b)
    write_hex32_arr(f, '%s_exp' % prefix, exp_res)

################################################################################
# Instructions with two read register and one immediate, with overwriting 
################################################################################
def instr_32_rrri(f, prefix, func, imm):
    
    ops_a    = []
    ops_d    = []
    exp_res  = []
    
    for i in range(0,10):
        a = random.randint(-2**31, 2**31-1)
        d = random.randint(-2**31, 2**31-1)

        r = func(a, imm, d)

        ops_a.append(a)
        ops_d.append(d)
        exp_res.append(r)
    
    write_hex32_arr(f, '%s_a'   % prefix, ops_a)
    write_hex32_arr(f, '%s_d'   % prefix, ops_d)
    write_hex32_arr(f, '%s_exp' % prefix, exp_res)

################################################################################
# Instructions with one read register and one immediate
################################################################################
def instr_32_ri(f, prefix, func, imm):
    
    ops_a    = []
    exp_res  = []
    
    for i in range(0,10):
        a = random.randint(-2**31, 2**31-1)

        r = func(a, imm)

        ops_a.append(a)
        exp_res.append(r)
    
    write_hex32_arr(f, '%s_a'   % prefix, ops_a)
    write_hex32_arr(f, '%s_exp' % prefix, exp_res)

def vec_16_ri_base(f, prefix, func, mina, maxa, imm):
    
    ops_a    = []
    exp_res  = []
    
    for i in range(0,10):
        op_a  = []
        res   = []
    
        for j in range(0,2):
            a = random.randint(mina, maxa)
    
            r = func(a, imm)
    
            op_a.append(a)
            res.append(r)

        ops_a.append(op_a)
        exp_res.append(res)
    
    write_hex16_arr(f, '%s_a'   % prefix, ops_a)
    write_hex16_arr(f, '%s_exp' % prefix, exp_res)

def vec_8_ri_base(f, prefix, func, mina, maxa, imm):
    
    ops_a    = []
    exp_res  = []
    
    for i in range(0,10):
        op_a  = []
        res   = []
    
        for j in range(0,4):
            a = random.randint(mina, maxa)
    
            r = func(a, imm)
    
            op_a.append(a)
            res.append(r)

        ops_a.append(op_a)
        exp_res.append(res)
    
    write_hex8_arr(f, '%s_a'   % prefix, ops_a)
    write_hex8_arr(f, '%s_exp' % prefix, exp_res)

def vec_8_ri(f, prefix, func, imm):
    vec_8_ri_base(f, prefix, func, -2**7, 2**7-1, imm)

def vec_16_ri(f, prefix, func, imm):
    vec_16_ri_base(f, prefix, func, -2**15, 2**15-1, imm)

################################################################################
if args.riscv:
    fa = open('testVecArith_stimuli_riscv.h', 'w')
else:
    fa = open('testVecArith_stimuli.h', 'w')


################################################################################
# generate testdata for l.add
################################################################################
instr_rr(fa, 'g_add', lambda a, b: (a + b))

################################################################################
# generate testdata for lv.add
################################################################################
vec_16_rr   (fa, 'g_add_h',      lambda a, b: (a + b))
vec_16_rr_sc(fa, 'g_add_h_sc',   lambda a, b: (a + b))
vec_16_rr_sci(fa, 'g_add_h_sci', lambda a, b: (a + b), sciImm)

vec_8_rr    (fa, 'g_add_b',     lambda a, b: (a + b))
vec_8_rr_sc (fa, 'g_add_b_sc',  lambda a, b: (a + b))
vec_8_rr_sci(fa, 'g_add_b_sci', lambda a, b: (a + b), sciImm)

################################################################################
# generate testdata for l.sub
################################################################################
instr_rr(fa, 'g_sub', lambda a, b: (a - b))

################################################################################
# generate testdata for lv.sub.h
################################################################################
vec_16_rr    (fa, 'g_sub_h',     lambda a, b: (a - b))
vec_16_rr_sc (fa, 'g_sub_h_sc',  lambda a, b: (a - b))
vec_16_rr_sci(fa, 'g_sub_h_sci', lambda a, b: (a - b), sciImm)

################################################################################
# generate testdata for lv.sub.b
################################################################################
vec_8_rr    (fa, 'g_sub_b',     lambda a, b: (a - b))
vec_8_rr_sc (fa, 'g_sub_b_sc',  lambda a, b: (a - b))
vec_8_rr_sci(fa, 'g_sub_b_sci', lambda a, b: (a - b), sciImm)

################################################################################
# generate testdata for l.abs
################################################################################

instr_rr(fa, 'g_abs',  lambda a, b: (-a if (a < 0) else a))

################################################################################
# generate testdata for lv.abs
################################################################################
vec_16_r(fa, 'g_abs_h', lambda a: (-a if (a < 0) else a))

vec_8_r(fa, 'g_abs_b', lambda a: (-a if (a < 0) else a))


################################################################################
# generate testdata for l.avg and l.avgu
################################################################################

def avg_32(a, b):
    r = ((a + b) & 0xFFFFFFFF) >> 1
    if(r >> 30):
        r = r | 0x80000000;

    return r

def avgu_32(a, b):
    r = ((a + b) & 0xFFFFFFFF) >> 1

    return r

instr_rr  (fa, 'g_avg',  avg_32)
instr_rr_u(fa, 'g_avgu', avgu_32)

################################################################################
# generate testdata for lv.avg.h and lv.avgu.h
################################################################################

def avg_16(a, b):
    r = ((a + b) & 0xFFFF) >> 1
    if(r >> 14):
        r = r | 0x8000;

    return r

def avgu_16(a, b):
    r = ((a + b) & 0xFFFF) >> 1

    return r

vec_16_rr    (fa, 'g_avg_h',     avg_16)
vec_16_rr_sc (fa, 'g_avg_h_sc',  avg_16)
vec_16_rr_sci(fa, 'g_avg_h_sci', avg_16, sciImm)
vec_16_rr    (fa, 'g_avgu_h',     avgu_16)
vec_16_rr_sc (fa, 'g_avgu_h_sc',  avgu_16)
vec_16_rr_sci(fa, 'g_avgu_h_sci', avgu_16, sciImm)

################################################################################
# generate testdata for lv.avg.b and lv.avgu.b
################################################################################
def avg_8(a, b):
    r = ((a + b) & 0xFF) >> 1
    if(r >> 6):
        r = r | 0x80;

    return r

def avgu_8(a, b):
    r = ((a + b) & 0xFF) >> 1

    return r

vec_8_rr    (fa, 'g_avg_b',     avg_8)
vec_8_rr_sc (fa, 'g_avg_b_sc',  avg_8)
vec_8_rr_sci(fa, 'g_avg_b_sci', avg_8, sciImm)
vec_8_rr    (fa, 'g_avgu_b',     avgu_8)
vec_8_rr_sc (fa, 'g_avgu_b_sc',  avgu_8)
vec_8_rr_sci(fa, 'g_avgu_b_sci', avgu_8, sciImm)

################################################################################
# generate testdata for lv.ext
################################################################################

def ext_16(a, L):
    if(L == 0):
        r = a
    else:
        r = a >> 16

    r = r & 0xFFFF
    if(r & 0x8000):
        r = r | 0xFFFF0000

    return r

def ext_8(a, L):
    if(L == 0):
        r = a
    elif(L == 1):
        r = a >> 8
    elif(L == 2):
        r = a >> 16
    else:
        r = a >> 24

    r = r & 0xFF
    if(r & 0x80):
        r = r | 0xFFFFFF00

    return r

instr_32_ri(fa,'g_ext_h0', ext_16, 0)
instr_32_ri(fa,'g_ext_h1', ext_16, 1)
instr_32_ri(fa,'g_ext_b0', ext_8,  0)
instr_32_ri(fa,'g_ext_b1', ext_8,  1)
instr_32_ri(fa,'g_ext_b2', ext_8,  2)
instr_32_ri(fa,'g_ext_b3', ext_8,  3)

################################################################################
# generate testdata for lv.extu
################################################################################

def extu_16(a, L):
    if(L == 0):
        r = a
    else:
        r = a >> 16

    r = r & 0xFFFF

    return r

def extu_8(a, L):
    if(L == 0):
        r = a
    elif(L == 1):
        r = a >> 8
    elif(L == 2):
        r = a >> 16
    else:
        r = a >> 24

    r = r & 0xFF

    return r

instr_32_ri(fa,'g_extu_h0', extu_16, 0)
instr_32_ri(fa,'g_extu_h1', extu_16, 1)
instr_32_ri(fa,'g_extu_b0', extu_8,  0)
instr_32_ri(fa,'g_extu_b1', extu_8,  1)
instr_32_ri(fa,'g_extu_b2', extu_8,  2)
instr_32_ri(fa,'g_extu_b3', extu_8,  3)

################################################################################
# generate testdata for pv.insert - Only on riscv
################################################################################

def ins_16(a, L, d):
    ins = (a & 0x0000FFFF)
    if(L == 0):
        r = (d & 0xFFFF0000) | ins
    else:
        r = (d & 0x0000FFFF) | (ins << 16)

    r = r & 0xFFFFFFFF

    return r

def ins_8(a, L, d):
    ins = (a & 0x000000FF)
    if(L == 0):
        r = (d & 0xFFFFFF00) | ins
    elif(L == 1):
        r = (d & 0xFFFF00FF) | (ins << 8)
    elif(L == 2):
        r = (d & 0xFF00FFFF) | (ins << 16)
    else:
        r = (d & 0x00FFFFFF) | (ins << 24)

    r = r & 0xFFFFFFFF

    return r

instr_32_rrri(fa,'g_ins_h0', ins_16, 0)
instr_32_rrri(fa,'g_ins_h1', ins_16, 1)
instr_32_rrri(fa,'g_ins_b0', ins_8,  0)
instr_32_rrri(fa,'g_ins_b1', ins_8,  1)
instr_32_rrri(fa,'g_ins_b2', ins_8,  2)
instr_32_rrri(fa,'g_ins_b3', ins_8,  3)