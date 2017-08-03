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

def write_define(f, name,val):
    f.write('#define %s %d\n\n' % (name,val))
    return


if args.riscv: f = open('testBitManipulation_stimuli_riscv.h', 'w')
else: f = open('testBitManipulation_stimuli.h', 'w')


NumberOfStimuli = 10

write_define(f, 'NumberOfStimuli',NumberOfStimuli)


upperboundA = 2**32-1
lowerboundA = 0

ops_a	 = []
ops_c    = []

exp_bset_res  = []
exp_bclr_res  = []
exp_bextract_res  = []
exp_bextractu_res  = []
exp_binsert_res  = []

for i in range(0,NumberOfStimuli):

    a = random.randint(lowerboundA, upperboundA)
    c = random.randint(lowerboundA, upperboundA)

    #imm + leng must be <= 32
    imm = random.randint(0, 31)

    for j in range(0,4): #5 tentatives
      if args.riscv: leng = random.randint(1, 32)
      else: leng = random.randint(0, 31)
      if leng + imm <= 32: break

    if leng + imm > 32:
      leng = 32 - imm

    Mask = (((1 << leng) -1 ) << imm) & 0xFFFFFFFF
    print 'Mask is %08x' % Mask
    imm_name  = 'IMM_' + str(i)
    leng_name = 'LEN_' + str(i)

    write_define(f, imm_name ,imm)

    # On Riscv the encoded length is the actual length minus 1
    if args.riscv: write_define(f, leng_name ,leng-1)
    else: write_define(f, leng_name ,leng)

    ops_a.append(a)
    ops_c.append(c)

    bset = a | Mask
    bclr = a & ~Mask

    bextract = (a & Mask)

    if leng + imm < 32: lshift = 32 - leng - imm
    else: lshift = 0

    bextract = bextract << lshift
    if(bextract & (1 << 31) != 0):
        bextract = bextract | 0xFFFFFFFF00000000

    bextract = (bextract >> (imm + lshift)) & 0xFFFFFFFF
    bextractu = ((a & Mask) >> imm) & 0xFFFFFFFF

    binsert = (((a << imm) & Mask) | (c & ~Mask)) & 0xFFFFFFFF

    exp_bset_res.append(bset)
    exp_bclr_res.append(bclr)
    exp_bextract_res.append(bextract)
    exp_bextractu_res.append(bextractu)
    exp_binsert_res.append(binsert)



write_hex32_arr(f, 'op_a' , ops_a)
write_hex32_arr(f, 'op_c' , ops_c)

write_hex32_arr(f, 'res_bset'     , exp_bset_res)
write_hex32_arr(f, 'res_bclr'     , exp_bclr_res)
write_hex32_arr(f, 'res_bextract' , exp_bextract_res)
write_hex32_arr(f, 'res_bextractu', exp_bextractu_res)
write_hex32_arr(f, 'res_binsert'  , exp_binsert_res)

ops_a    = []
ops_b    = []
ops_c    = []

exp_bset_res  = []
exp_bclr_res  = []
exp_bextract_res  = []
exp_bextractu_res  = []
exp_binsert_res  = []

for i in range(0,NumberOfStimuli):

    a = random.randint(lowerboundA, upperboundA)
    c = random.randint(lowerboundA, upperboundA)

    #imm + leng must be <= 32
    imm = random.randint(0, 31)

    for j in range(0,4): #5 tentatives
      if args.riscv: leng = random.randint(1, 32)
      else: leng = random.randint(0, 31)
      if leng + imm <= 32: break

    if leng + imm > 32:
      leng = 32 - imm

    Mask = (((1 << leng) -1 ) << imm) & 0xFFFFFFFF
    print 'Mask is %08x' % Mask

    b = imm | ((leng-1) << 5)

    ops_a.append(a)
    ops_b.append(b)
    ops_c.append(c)

    bset = a | Mask
    bclr = a & ~Mask

    bextract = (a & Mask)

    if leng + imm < 32: lshift = 32 - leng - imm
    else: lshift = 0

    bextract = bextract << lshift
    if(bextract & (1 << 31) != 0):
        bextract = bextract | 0xFFFFFFFF00000000

    bextract = (bextract >> (imm + lshift)) & 0xFFFFFFFF
    bextractu = ((a & Mask) >> imm) & 0xFFFFFFFF

    binsert = (((a << imm) & Mask) | (c & ~Mask)) & 0xFFFFFFFF

    exp_bset_res.append(bset)
    exp_bclr_res.append(bclr)
    exp_bextract_res.append(bextract)
    exp_bextractu_res.append(bextractu)
    exp_binsert_res.append(binsert)



write_hex32_arr(f, 'op_a_reg' , ops_a)
write_hex32_arr(f, 'op_b_reg' , ops_b)
write_hex32_arr(f, 'op_c_reg' , ops_c)

write_hex32_arr(f, 'res_bset_reg'     , exp_bset_res)
write_hex32_arr(f, 'res_bclr_reg'     , exp_bclr_res)
write_hex32_arr(f, 'res_bextract_reg' , exp_bextract_res)
write_hex32_arr(f, 'res_bextractu_reg', exp_bextractu_res)
write_hex32_arr(f, 'res_binsert_reg'  , exp_binsert_res)
