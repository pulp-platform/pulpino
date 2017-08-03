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


def switch_case8(sel,val0,val1,val2,val3):
    r = 0
    if(sel == 3):
       r = val3
    elif(sel == 2):
       r = val2
    elif(sel == 1):
       r = val1
    else:
       r = val0
    return r

def write_define(f, name,val):
    f.write('#define %s %d\n\n' % (name,val))
    return

NumberOfStimuli = 10



################################################################################
# Shuffle instructions
################################################################################
def shuffle_16(prefix):
    global f

    ops_a    = []
    ops_b    = []
    exp_res  = []


    for i in range(0,NumberOfStimuli):

        a = random.randint(0, 2**32-1)
        b = random.randint(0, 2**32-1)

        h = (b & 0x10000) >> 16
        l = (b & 0x1)


        sel1  = (a & 0xffff0000) >> 16
        sel0  = a & 0x0000ffff
        r = 0

        if(h):
          rh = sel1
        else:
          rh = sel0

        if(l):
          rl = sel1
        else:
          rl = sel0

        r = (rh << 16) | rl;

        ops_a.append(a)
        ops_b.append(b)
        exp_res.append(r)

    write_hex32_arr(f, '%s_a'   % prefix, ops_a)
    write_hex32_arr(f, '%s_b'   % prefix, ops_b)
    write_hex32_arr(f, '%s_exp' % prefix, exp_res)

def shuffle_sci_16(prefix):
    global f

    ops_a    = []
    ops_b    = []
    exp_res  = []

    for i in range(0,10):

        a   = random.randint(0, 2**32-1)
        if args.riscv: imm = random.randint(0, 2**5-1)
        else: imm = random.randint(0, 2**7-1)

        h = (imm & 0x2) >> 1
        l = (imm & 0x1)

        sel1  = (a & 0xffff0000) >> 16
        sel0  = a & 0x0000ffff

        r = 0

        if(h):
          rh = sel1
        else:
          rh = sel0

        if(l):
          rl = sel1
        else:
          rl = sel0

        r = (rh << 16) | rl;

        ops_a.append(a)
        imm_name = prefix + '_' + str(i)
        write_define(f, imm_name, imm)
        exp_res.append(r)

    write_hex32_arr(f, '%s_a'   % prefix, ops_a)
    write_hex32_arr(f, '%s_exp' % prefix, exp_res)


def shuffle_8(prefix):
    global f

    ops_a    = []
    ops_b    = []
    exp_res  = []


    for i in range(0,NumberOfStimuli):

        a = random.randint(0, 2**32-1)
        b = random.randint(0, 2**32-1)

        hh = (b & 0x3000000) >> 24
        hl = (b & 0x0030000) >> 16
        lh = (b & 0x0000300) >> 8
        ll = (b & 0x0000003)

        sel3 = (a & 0xff000000) >> 24
        sel2 = (a & 0x00ff0000) >> 16
        sel1 = (a & 0x0000ff00) >> 8
        sel0 = (a & 0x000000ff)


        r = 0

        rhh = switch_case8(hh,sel0,sel1,sel2,sel3)

        rhl = switch_case8(hl,sel0,sel1,sel2,sel3)

        rlh = switch_case8(lh,sel0,sel1,sel2,sel3)

        rll = switch_case8(ll,sel0,sel1,sel2,sel3)

        r = (rhh << 24) | (rhl << 16) | (rlh << 8) | (rll);


        ops_a.append(a)
        ops_b.append(b)
        exp_res.append(r)

    write_hex32_arr(f, '%s_a'   % prefix, ops_a)
    write_hex32_arr(f, '%s_b'   % prefix, ops_b)
    write_hex32_arr(f, '%s_exp' % prefix, exp_res)

def shuffle_sci_8(prefix):
    global f

    ops_a    = []
    ops_b    = []
    exp_res  = []


    for i in range(0,40):

        a = random.randint(0, 2**32-1)
        imm = random.randint(0, 2**7-1)

        hh = (imm & 0xC0) >> 6
        hl = (imm & 0x30) >> 4
        lh = (imm & 0x0C) >> 2
        ll = (imm & 0x3)

        sel3 = (a & 0xff000000) >> 24
        sel2 = (a & 0x00ff0000) >> 16
        sel1 = (a & 0x0000ff00) >> 8
        sel0 = (a & 0x000000ff)

        r = 0

        rhh = switch_case8(hh,sel0,sel1,sel2,sel3)

        rhl = switch_case8(hl,sel0,sel1,sel2,sel3)

        rlh = switch_case8(lh,sel0,sel1,sel2,sel3)

        rll = switch_case8(ll,sel0,sel1,sel2,sel3)

        r = (rhh << 24) | (rhl << 16) | (rlh << 8) | (rll);


        ops_a.append(a)
        imm_name = prefix + '_' + str(i)
        write_define(f, imm_name, imm)
        exp_res.append(r)

    write_hex32_arr(f, '%s_a'   % prefix, ops_a)
    write_hex32_arr(f, '%s_exp' % prefix, exp_res)


def shuffle2_16(prefix):
    global f

    ops_a    = []
    ops_b    = []
    ops_c    = []
    exp_res  = []


    for i in range(0,NumberOfStimuli):

        a = random.randint(0, 2**32-1)
        b = random.randint(0, 2**32-1)
        c = random.randint(0, 2**32-1)

        h = (b & 0x10000) >> 16
        l = (b & 0x1)

        AorD_h = b & 0x20000 #17
        AorD_l = b & 0x2 #01

        #RTL GCC misalignemnt on orion
        if(AorD_h):
           oph  = a if args.riscv else c
        else:
           oph  = c if args.riscv else a


        if(AorD_l):
           opl  = a if args.riscv else c
        else:
           opl  = c if args.riscv else a

        r = 0

        if(h):
          rh = (oph & 0xffff0000) >> 16
        else:
          rh = oph & 0x0000ffff

        if(l):
          rl = (opl & 0xffff0000) >> 16
        else:
          rl = opl & 0x0000ffff

        r = (rh << 16) | rl;

        ops_a.append(a)
        ops_b.append(b)
        ops_c.append(c)
        exp_res.append(r)

    write_hex32_arr(f, '%s_a'   % prefix, ops_a)
    write_hex32_arr(f, '%s_b'   % prefix, ops_b)
    write_hex32_arr(f, '%s_c'   % prefix, ops_c)
    write_hex32_arr(f, '%s_exp' % prefix, exp_res)

def shuffle2_8(prefix):
    global f

    ops_a    = []
    ops_b    = []
    ops_c    = []
    exp_res  = []


    for i in range(0,NumberOfStimuli):

        a = random.randint(0, 2**32-1)
        b = random.randint(0, 2**32-1)
        c = random.randint(0, 2**32-1)

        AorD_hh = b & 0x4000000 #26
        AorD_hl = b & 0x40000     #18
        AorD_lh = b & 0x400   #10
        AorD_ll = b & 0x4     #02

        hh = (b & 0x3000000) >> 24
        hl = (b & 0x0030000) >> 16
        lh = (b & 0x0000300) >> 8
        ll = (b & 0x0000003)

        #RTL GCC misalignemnt on orion
        if(AorD_hh):
          ophh = a if args.riscv else c
        else:
          ophh = c if args.riscv else a

        if(AorD_hl):
          ophl = a if args.riscv else c
        else:
          ophl = c if args.riscv else a

        if(AorD_lh):
          oplh = a if args.riscv else c
        else:
          oplh = c if args.riscv else a

        if(AorD_ll):
          opll = a if args.riscv else c
        else:
          opll = c if args.riscv else a

        sel3 = (ophh & 0xff000000) >> 24
        sel2 = (ophh & 0x00ff0000) >> 16
        sel1 = (ophh & 0x0000ff00) >> 8
        sel0 = (ophh & 0x000000ff)

        r = 0

        rhh = switch_case8(hh,sel0,sel1,sel2,sel3)

        sel3 = (ophl & 0xff000000) >> 24
        sel2 = (ophl & 0x00ff0000) >> 16
        sel1 = (ophl & 0x0000ff00) >> 8
        sel0 = (ophl & 0x000000ff)

        rhl = switch_case8(hl,sel0,sel1,sel2,sel3)

        sel3 = (oplh & 0xff000000) >> 24
        sel2 = (oplh & 0x00ff0000) >> 16
        sel1 = (oplh & 0x0000ff00) >> 8
        sel0 = (oplh & 0x000000ff)

        rlh = switch_case8(lh,sel0,sel1,sel2,sel3)

        sel3 = (opll & 0xff000000) >> 24
        sel2 = (opll & 0x00ff0000) >> 16
        sel1 = (opll & 0x0000ff00) >> 8
        sel0 = (opll & 0x000000ff)

        rll = switch_case8(ll,sel0,sel1,sel2,sel3)

        r = (rhh << 24) | (rhl << 16) | (rlh << 8) | (rll);


        ops_a.append(a)
        ops_b.append(b)
        ops_c.append(c)
        exp_res.append(r)

    write_hex32_arr(f, '%s_a'   % prefix, ops_a)
    write_hex32_arr(f, '%s_b'   % prefix, ops_b)
    write_hex32_arr(f, '%s_c'   % prefix, ops_c)
    write_hex32_arr(f, '%s_exp' % prefix, exp_res)

################################################################################
# Pack instructions
################################################################################
def pack_16(prefix):
    global f

    ops_a    = []
    ops_b    = []
    exp_res  = []


    for i in range(0,NumberOfStimuli):

        a = random.randint(0, 2**32-1)
        b = random.randint(0, 2**32-1)

        rh  = a & 0x0000ffff
        rl  = b & 0x0000ffff

        r = (rh << 16) | rl;

        ops_a.append(a)
        ops_b.append(b)
        exp_res.append(r)

    write_hex32_arr(f, '%s_a'   % prefix, ops_a)
    write_hex32_arr(f, '%s_b'   % prefix, ops_b)
    write_hex32_arr(f, '%s_exp' % prefix, exp_res)

def pack_8_hi(prefix):
    global f

    ops_a    = []
    ops_b    = []
    ops_c    = []
    exp_res  = []


    for i in range(0,NumberOfStimuli):

        a = random.randint(0, 2**32-1)
        b = random.randint(0, 2**32-1)
        c = random.randint(0, 2**32-1)

        rhh  = a & 0x000000ff
        rhl  = b & 0x000000ff
        rl   = c & 0x0000ffff

        r = (rhh << 24) | (rhl << 16) | rl;

        ops_a.append(a)
        ops_b.append(b)
        ops_c.append(c)
        exp_res.append(r)

    write_hex32_arr(f, '%s_a'   % prefix, ops_a)
    write_hex32_arr(f, '%s_b'   % prefix, ops_b)
    write_hex32_arr(f, '%s_c'   % prefix, ops_c)
    write_hex32_arr(f, '%s_exp' % prefix, exp_res)

def pack_8_lo(prefix):
    global f

    ops_a    = []
    ops_b    = []
    ops_c    = []
    exp_res  = []


    for i in range(0,NumberOfStimuli):

        a = random.randint(0, 2**32-1)
        b = random.randint(0, 2**32-1)
        c = random.randint(0, 2**32-1)

        rhh  = a & 0x000000ff
        rhl  = b & 0x000000ff
        rl   = c & 0xffff0000

        r = (rhh << 8) | rhl | rl;

        ops_a.append(a)
        ops_b.append(b)
        ops_c.append(c)
        exp_res.append(r)

    write_hex32_arr(f, '%s_a'   % prefix, ops_a)
    write_hex32_arr(f, '%s_b'   % prefix, ops_b)
    write_hex32_arr(f, '%s_c'   % prefix, ops_c)
    write_hex32_arr(f, '%s_exp' % prefix, exp_res)

############################################################
if args.riscv: f = open('testShufflePack_stimuli_riscv.h', 'w')
else: f = open('testShufflePack_stimuli.h', 'w')
write_define(f, 'NumberOfStimuli',NumberOfStimuli)


shuffle_16     ('g_shuffle_h')
shuffle_8      ('g_shuffle_b')
shuffle_sci_16 ('g_shuffle_sci_h')
shuffle_sci_8  ('g_shuffle_sci_b')

shuffle2_16    ('g_shuffle2_h')
shuffle2_8     ('g_shuffle2_b')

pack_16        ('g_pack_h')
pack_8_hi      ('g_pack_hi_b')
pack_8_lo      ('g_pack_lo_b')
