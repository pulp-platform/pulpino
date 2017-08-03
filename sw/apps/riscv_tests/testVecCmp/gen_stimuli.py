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
# Comparison instructions
################################################################################
def cmp_16_rr(prefix, func, sign):
    global f

    ops_a    = []
    ops_b    = []
    exp_res  = []
    
    lowerbound = 0
    upperbound = 0
    if(sign):
        lowerbound = -2**15
        upperbound = 2**15 -1
    else:
        lowerbound = 0
        upperbound = 2**16 -1

    for i in range(0,10):
        op_a  = []
        op_b  = []
        res   = []
    
        for j in range(0,2):
            b = random.randint(lowerbound+1, upperbound-1)
            if(i % 4 == 0):
                a = random.randint(lowerbound, upperbound)
            elif(i % 4 == 1):
                a = b
            elif(i % 4 == 2):
                a = b - 1
            else:
                a = b + 1
    
            if( func(a, b) ):
              r = 0xFFFF
            else:
              r = 0;
    
            op_a.append(a)
            op_b.append(b)
            res.append(r)

        ops_a.append(op_a)
        ops_b.append(op_b)
        exp_res.append(res)
    
    write_hex16_arr(f, '%s_a'   % prefix, ops_a)
    write_hex16_arr(f, '%s_b'   % prefix, ops_b)
    write_hex16_arr(f, '%s_act' % prefix, ops_a)
    write_hex16_arr(f, '%s_exp' % prefix, exp_res)

def cmp_16_rr_sc(prefix, func, sign):
    global f

    ops_a    = []
    ops_b    = []
    exp_res  = []
    
    lowerbound = 0
    upperbound = 0
    if(sign):
        lowerbound = -2**15
        upperbound = 2**15 -1
    else:
        lowerbound = 0
        upperbound = 2**16 -1

    for i in range(0,10):
        op_a  = []
        op_b  = []
        res   = []

        b = random.randint(lowerbound+1, upperbound-1)

        for j in range(0,2):
            if(i % 4 == 0):
                a = random.randint(lowerbound, upperbound)
            elif(i % 4 == 1):
                a = b
            elif(i % 4 == 2):
                a = b - 1
            else:
                a = b + 1
    
            if( func(a, b) ):
              r = 0xFFFF
            else:
              r = 0;
    
            op_a.append(a)
            res.append(r)

            if(j == 1):
                op_b.append(b)
            else:
                op_b.append(random.randint(-2**15, 2**15-1))

        ops_a.append(op_a)
        ops_b.append(op_b)
        exp_res.append(res)
    
    write_hex16_arr(f, '%s_a'   % prefix, ops_a)
    write_hex16_arr(f, '%s_b'   % prefix, ops_b)
    write_hex16_arr(f, '%s_act' % prefix, ops_a)
    write_hex16_arr(f, '%s_exp' % prefix, exp_res)

def cmp_16_rr_sci(prefix, func, imm, sign):
    global f

    ops_a    = []
    exp_res  = []
    
    lowerbound = 0
    upperbound = 0
    if(sign):
        lowerbound = -2**15
        upperbound = 2**15 -1
    else:
        lowerbound = 0
        upperbound = 2**16 -1

    for i in range(0,10):
        op_a  = []
        res   = []

        for j in range(0,2):
            if(i % 4 == 0):
                a = random.randint(lowerbound, upperbound)
            elif(i % 4 == 1):
                a = imm
            elif(i % 4 == 2):
                a = imm - 1
            else:
                a = imm + 1
    
            if( func(a, imm) ):
              r = 0xFFFF
            else:
              r = 0;
    
            op_a.append(a)
            res.append(r)

        ops_a.append(op_a)
        exp_res.append(res)
    
    write_hex16_arr(f, '%s_a'   % prefix, ops_a)
    write_hex16_arr(f, '%s_act' % prefix, ops_a)
    write_hex16_arr(f, '%s_exp' % prefix, exp_res)

def cmp_8_rr(prefix, func,sign):
    global f

    ops_a    = []
    ops_b    = []
    exp_res  = []
    
    lowerbound = 0
    upperbound = 0
    if(sign):
        lowerbound = -2**7
        upperbound = 2**7 -1
    else:
        lowerbound = 0
        upperbound = 2**8 -1

    for i in range(0,10):
        op_a  = []
        op_b  = []
        res   = []
    
        for j in range(0,4):
            b = random.randint(lowerbound+1, upperbound-1)
            if(i % 4 == 0):
                a = random.randint(lowerbound, upperbound)
            elif(i % 4 == 1):
                a = b
            elif(i % 4 == 2):
                a = b - 1
            else:
                a = b + 1
    
            if( func(a, b) ):
              r = 0xFF
            else:
              r = 0;
    
            op_a.append(a)
            op_b.append(b)
            res.append(r)

        ops_a.append(op_a)
        ops_b.append(op_b)
        exp_res.append(res)
    
    print "Prefix is ", '%s_a'   % prefix
    write_hex8_arr(f, '%s_a'   % prefix, ops_a)
    write_hex8_arr(f, '%s_b'   % prefix, ops_b)
    write_hex8_arr(f, '%s_act' % prefix, ops_a)
    write_hex8_arr(f, '%s_exp' % prefix, exp_res)

def cmp_8_rr_sc(prefix, func, sign):
    global f

    ops_a    = []
    ops_b    = []
    exp_res  = []
    
    lowerbound = 0
    upperbound = 0
    if(sign):
        lowerbound = -2**7
        upperbound = 2**7 -1
    else:
        lowerbound = 0
        upperbound = 2**8 -1

    for i in range(0,10):
        op_a  = []
        op_b  = []
        res   = []

        b = random.randint(lowerbound+1, upperbound-1)
    
        for j in range(0,4):
            if(i % 4 == 0):
                a = random.randint(lowerbound, upperbound)
            elif(i % 4 == 1):
                a = b
            elif(i % 4 == 2):
                a = b - 1
            else:
                a = b + 1
    
            if( func(a, b) ):
              r = 0xFF
            else:
              r = 0;
    
            op_a.append(a)
            res.append(r)

            if(j == 3):
                op_b.append(b)
            else:
                op_b.append(random.randint(-2**7, 2**7-1)) # just a random value, we don't care

        ops_a.append(op_a)
        ops_b.append(op_b)
        exp_res.append(res)
    
    write_hex8_arr(f, '%s_a'   % prefix, ops_a)
    write_hex8_arr(f, '%s_b'   % prefix, ops_b)
    write_hex8_arr(f, '%s_act' % prefix, ops_a)
    write_hex8_arr(f, '%s_exp' % prefix, exp_res)

def cmp_8_rr_sci(prefix, func, imm, sign):
    global f

    ops_a    = []
    exp_res  = []
    
    lowerbound = 0
    upperbound = 0
    if(sign):
        lowerbound = -2**7
        upperbound = 2**7 -1
    else:
        lowerbound = 0
        upperbound = 2**8 -1

    for i in range(0,10):
        op_a  = []
        res   = []

        for j in range(0,4):
            if(i % 4 == 0):
                a = random.randint(lowerbound, upperbound)
            elif(i % 4 == 1):
                a = imm
            elif(i % 4 == 2):
                a = imm - 1
            else:
                a = imm + 1
    
            if( func(a, imm) ):
              r = 0xFF
            else:
              r = 0;
    
            op_a.append(a)
            res.append(r)

        ops_a.append(op_a)
        exp_res.append(res)
    
    write_hex8_arr(f, '%s_a'   % prefix, ops_a)
    write_hex8_arr(f, '%s_act' % prefix, ops_a)
    write_hex8_arr(f, '%s_exp' % prefix, exp_res)

################################################################################
if args.riscv: f = open('testVecCmp_stimuli_riscv.h', 'w')
else: f = open('testVecCmp_stimuli.h', 'w')

################################################################################
# lv.cmp_eq
################################################################################
cmp_16_rr    ('g_eq_h',     lambda a, b: (a == b),1)
cmp_16_rr_sc ('g_eq_h_sc',  lambda a, b: (a == b),1)
cmp_16_rr_sci('g_eq_h_sci', lambda a, b: (a == b), sciImm,1)

cmp_8_rr     ('g_eq_b',     lambda a, b: (a == b),1)
cmp_8_rr_sc  ('g_eq_b_sc',  lambda a, b: (a == b),1)
cmp_8_rr_sci ('g_eq_b_sci', lambda a, b: (a == b), sciImm,1)

################################################################################
# lv.cmp_ne
################################################################################
cmp_16_rr    ('g_ne_h',     lambda a, b: (a != b),1)
cmp_16_rr_sc ('g_ne_h_sc',  lambda a, b: (a != b),1)
cmp_16_rr_sci('g_ne_h_sci', lambda a, b: (a != b), sciImm,1)

cmp_8_rr     ('g_ne_b',     lambda a, b: (a != b),1)
cmp_8_rr_sc  ('g_ne_b_sc',  lambda a, b: (a != b),1)
cmp_8_rr_sci ('g_ne_b_sci', lambda a, b: (a != b), sciImm,1)

################################################################################
# lv.cmp_le
################################################################################
cmp_16_rr    ('g_le_h',     lambda a, b: (a <= b),1)
cmp_16_rr_sc ('g_le_h_sc',  lambda a, b: (a <= b),1)
cmp_16_rr_sci('g_le_h_sci', lambda a, b: (a <= b), sciImm,1)

cmp_8_rr     ('g_le_b',     lambda a, b: (a <= b),1)
cmp_8_rr_sc  ('g_le_b_sc',  lambda a, b: (a <= b),1)
cmp_8_rr_sci ('g_le_b_sci', lambda a, b: (a <= b), sciImm,1)

################################################################################
# lv.cmp_leu
################################################################################

cmp_16_rr    ('g_leu_h',     lambda a, b: (a <= b),0)
cmp_16_rr_sc ('g_leu_h_sc',  lambda a, b: (a <= b),0)
cmp_16_rr_sci('g_leu_h_sci', lambda a, b: (a <= b), sciImm,0)

cmp_8_rr     ('g_leu_b',     lambda a, b: (a <= b),0)
cmp_8_rr_sc  ('g_leu_b_sc',  lambda a, b: (a <= b),0)
cmp_8_rr_sci ('g_leu_b_sci', lambda a, b: (a <= b), sciImm,0)


################################################################################
# lv.cmp_lt
################################################################################
cmp_16_rr    ('g_lt_h',     lambda a, b: (a < b),1)
cmp_16_rr_sc ('g_lt_h_sc',  lambda a, b: (a < b),1)
cmp_16_rr_sci('g_lt_h_sci', lambda a, b: (a < b), sciImm,1)

cmp_8_rr     ('g_lt_b',     lambda a, b: (a < b),1)
cmp_8_rr_sc  ('g_lt_b_sc',  lambda a, b: (a < b),1)
cmp_8_rr_sci ('g_lt_b_sci', lambda a, b: (a < b), sciImm,1)

################################################################################
# lv.cmp_ltu
################################################################################
cmp_16_rr    ('g_ltu_h',     lambda a, b: (a < b),0)
cmp_16_rr_sc ('g_ltu_h_sc',  lambda a, b: (a < b),0)
cmp_16_rr_sci('g_ltu_h_sci', lambda a, b: (a < b), sciImm,0)

cmp_8_rr     ('g_ltu_b',     lambda a, b: (a < b),0)
cmp_8_rr_sc  ('g_ltu_b_sc',  lambda a, b: (a < b),0)
cmp_8_rr_sci ('g_ltu_b_sci', lambda a, b: (a < b), sciImm,0)

################################################################################
# lv.cmp_ge
################################################################################
cmp_16_rr    ('g_ge_h',     lambda a, b: (a >= b),1)
cmp_16_rr_sc ('g_ge_h_sc',  lambda a, b: (a >= b),1)
cmp_16_rr_sci('g_ge_h_sci', lambda a, b: (a >= b), sciImm,1)

cmp_8_rr     ('g_ge_b',     lambda a, b: (a >= b),1)
cmp_8_rr_sc  ('g_ge_b_sc',  lambda a, b: (a >= b),1)
cmp_8_rr_sci ('g_ge_b_sci', lambda a, b: (a >= b), sciImm,1)

################################################################################
# lv.cmp_geu
################################################################################
cmp_16_rr    ('g_geu_h',     lambda a, b: (a >= b),0)
cmp_16_rr_sc ('g_geu_h_sc',  lambda a, b: (a >= b),0)
cmp_16_rr_sci('g_geu_h_sci', lambda a, b: (a >= b), sciImm,0)

cmp_8_rr     ('g_geu_b',     lambda a, b: (a >= b),0)
cmp_8_rr_sc  ('g_geu_b_sc',  lambda a, b: (a >= b),0)
cmp_8_rr_sci ('g_geu_b_sci', lambda a, b: (a >= b), sciImm,0)

################################################################################
# lv.cmp_gt
################################################################################
cmp_16_rr    ('g_gt_h',     lambda a, b: (a > b),1)
cmp_16_rr_sc ('g_gt_h_sc',  lambda a, b: (a > b),1)
cmp_16_rr_sci('g_gt_h_sci', lambda a, b: (a > b), sciImm,1)

cmp_8_rr     ('g_gt_b',     lambda a, b: (a > b),1)
cmp_8_rr_sc  ('g_gt_b_sc',  lambda a, b: (a > b),1)
cmp_8_rr_sci ('g_gt_b_sci', lambda a, b: (a > b), sciImm,1)

################################################################################
# lv.cmp_gtu
################################################################################
cmp_16_rr    ('g_gtu_h',     lambda a, b: (a > b),0)
cmp_16_rr_sc ('g_gtu_h_sc',  lambda a, b: (a > b),0)
cmp_16_rr_sci('g_gtu_h_sci', lambda a, b: (a > b), sciImm,0)

cmp_8_rr     ('g_gtu_b',     lambda a, b: (a > b),0)
cmp_8_rr_sc  ('g_gtu_b_sc',  lambda a, b: (a > b),0)
cmp_8_rr_sci ('g_gtu_b_sci', lambda a, b: (a > b), sciImm,0)