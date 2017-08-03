#!/usr/bin/env python-2.5

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

f = open('testALU_stimuli.h', 'w')


# number of test cases to generate
N = 15

################################################################################
################################################################################
#
# ADDITION
#
################################################################################
################################################################################

################################################################################
# generate testdata for l.add
################################################################################
ops_a    = []
ops_b    = []
exp_res  = []

for i in range(N):
    a = random.randint(-2**31, 2**31-1)
    b = random.randint(-2**31, 2**31-1)

    r = a + b

    ops_a.append(a)
    ops_b.append(b)
    exp_res.append(r)

write_hex32_arr(f, 'g_add_a', ops_a)
write_hex32_arr(f, 'g_add_b', ops_b)
write_hex32_arr(f, 'g_add_act', ops_a)
write_hex32_arr(f, 'g_add_exp', exp_res)

################################################################################
# generate testdata for l.addi
################################################################################
ops_a    = []
exp_res  = []

for i in range(N):
    a = random.randint(-2**31, 2**31-1)

    r = a + 100

    ops_a.append(a)
    exp_res.append(r)

write_hex32_arr(f, 'g_addi_a', ops_a)
write_hex32_arr(f, 'g_addi_act', ops_a)
write_hex32_arr(f, 'g_addi_exp', exp_res)


################################################################################
################################################################################
#
# SUBTRACTION
#
################################################################################
################################################################################

################################################################################
# generate testdata for l.sub
################################################################################
ops_a    = []
ops_b    = []
exp_res  = []

for i in range(N):
    a = random.randint(-2**31, 2**31-1)
    b = random.randint(-2**31, 2**31-1)

    r = a - b

    ops_a.append(a)
    ops_b.append(b)
    exp_res.append(r)

write_hex32_arr(f, 'g_sub_a', ops_a)
write_hex32_arr(f, 'g_sub_b', ops_b)
write_hex32_arr(f, 'g_sub_act', ops_a)
write_hex32_arr(f, 'g_sub_exp', exp_res)

################################################################################
################################################################################
#
# LOGIC STUFF
#
################################################################################
################################################################################

################################################################################
# generate testdata for l.and
################################################################################
ops_a    = []
ops_b    = []
exp_res  = []

for i in range(N):
    a = random.randint(-2**31, 2**31-1)
    b = random.randint(-2**31, 2**31-1)

    r = a & b

    ops_a.append(a)
    ops_b.append(b)
    exp_res.append(r)

write_hex32_arr(f, 'g_and_a', ops_a)
write_hex32_arr(f, 'g_and_b', ops_b)
write_hex32_arr(f, 'g_and_act', ops_a)
write_hex32_arr(f, 'g_and_exp', exp_res)

################################################################################
# generate testdata for l.or
################################################################################
ops_a    = []
ops_b    = []
exp_res  = []

for i in range(N):
    a = random.randint(-2**31, 2**31-1)
    b = random.randint(-2**31, 2**31-1)

    r = a | b

    ops_a.append(a)
    ops_b.append(b)
    exp_res.append(r)

write_hex32_arr(f, 'g_or_a', ops_a)
write_hex32_arr(f, 'g_or_b', ops_b)
write_hex32_arr(f, 'g_or_act', ops_a)
write_hex32_arr(f, 'g_or_exp', exp_res)

################################################################################
# generate testdata for l.xor
################################################################################
ops_a    = []
ops_b    = []
exp_res  = []

for i in range(N):
    a = random.randint(-2**31, 2**31-1)
    b = random.randint(-2**31, 2**31-1)

    r = a ^ b

    ops_a.append(a)
    ops_b.append(b)
    exp_res.append(r)

write_hex32_arr(f, 'g_xor_a', ops_a)
write_hex32_arr(f, 'g_xor_b', ops_b)
write_hex32_arr(f, 'g_xor_act', ops_a)
write_hex32_arr(f, 'g_xor_exp', exp_res)

################################################################################
################################################################################
#
# Shifting
#
################################################################################
################################################################################

################################################################################
# generate testdata for l.sll
################################################################################
ops_a    = []
ops_b    = []
exp_res  = []

for i in range(N):
    a = random.randint(-2**31, 2**31-1)
    b = random.randint(0, 31)

    r = a << b

    ops_a.append(a)
    ops_b.append(b)
    exp_res.append(r)

write_hex32_arr(f, 'g_sll_a', ops_a)
write_hex32_arr(f, 'g_sll_b', ops_b)
write_hex32_arr(f, 'g_sll_act', ops_a)
write_hex32_arr(f, 'g_sll_exp', exp_res)


################################################################################
# generate testdata for l.srl
################################################################################
ops_a    = []
ops_b    = []
exp_res  = []

for i in range(N):
    a = random.randint(-2**31, 2**31-1)
    b = random.randint(0, 31)

    # python does not provide logical right shifts, so we have to hack
    # something together
    r = (a % 0x100000000) >> b

    ops_a.append(a)
    ops_b.append(b)
    exp_res.append(r)

write_hex32_arr(f, 'g_srl_a', ops_a)
write_hex32_arr(f, 'g_srl_b', ops_b)
write_hex32_arr(f, 'g_srl_act', ops_a)
write_hex32_arr(f, 'g_srl_exp', exp_res)


################################################################################
# generate testdata for l.sra
################################################################################
ops_a    = []
ops_b    = []
exp_res  = []

for i in range(N):
    a = random.randint(-2**31, 2**31-1)
    b = random.randint(0, 31)

    r = a >> b

    ops_a.append(a)
    ops_b.append(b)
    exp_res.append(r)

write_hex32_arr(f, 'g_sra_a', ops_a)
write_hex32_arr(f, 'g_sra_b', ops_b)
write_hex32_arr(f, 'g_sra_act', ops_a)
write_hex32_arr(f, 'g_sra_exp', exp_res)


################################################################################
# generate testdata for l.slli
################################################################################
ops_a    = []
exp_res  = []

for i in range(N):
    a = random.randint(-2**31, 2**31-1)

    r = a << 10

    ops_a.append(a)
    exp_res.append(r)

write_hex32_arr(f, 'g_slli_a', ops_a)
write_hex32_arr(f, 'g_slli_act', ops_a)
write_hex32_arr(f, 'g_slli_exp', exp_res)


################################################################################
# generate testdata for l.srli
################################################################################
ops_a    = []
exp_res  = []

for i in range(N):
    a = random.randint(-2**31, 2**31-1)

    # python does not provide logical right shifts, so we have to hack
    # something together
    r = (a % 0x100000000) >> 9

    ops_a.append(a)
    exp_res.append(r)

write_hex32_arr(f, 'g_srli_a', ops_a)
write_hex32_arr(f, 'g_srli_act', ops_a)
write_hex32_arr(f, 'g_srli_exp', exp_res)


################################################################################
# generate testdata for l.srai
################################################################################
ops_a    = []
exp_res  = []

for i in range(N):
    a = random.randint(-2**31, 2**31-1)

    r = a >> 21

    ops_a.append(a)
    exp_res.append(r)

write_hex32_arr(f, 'g_srai_a', ops_a)
write_hex32_arr(f, 'g_srai_act', ops_a)
write_hex32_arr(f, 'g_srai_exp', exp_res)


# ################################################################################
# # generate testdata for l.cmov
# ################################################################################
# ops_a    = []
# ops_b    = []
# exp_res  = []
#
# for i in range(N):
#     a = random.randint(0, 2**32-1)
#     b = random.randint(0, 2**32-1)
#
#     if(a > b):
#         r = a
#     else:
#         r = b
#
#     ops_a.append(a)
#     ops_b.append(b)
#     exp_res.append(r)
#
# write_hex32_arr(f, 'g_cmov_a', ops_a)
# write_hex32_arr(f, 'g_cmov_b', ops_b)
# write_hex32_arr(f, 'g_cmov_act', ops_a)
# write_hex32_arr(f, 'g_cmov_exp', exp_res)
