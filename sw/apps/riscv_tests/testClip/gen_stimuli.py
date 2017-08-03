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

def write_define(f, name,val):
    f.write('#define %s %d\n\n' % (name,val))
    return


f = open('testClip_stimuli.h', 'w')


NumberOfStimuli = 10

write_define(f, 'NumberOfStimuli',NumberOfStimuli)





upperboundA = 2**32-1
lowerboundA = 0

ops_a_s	 = []
ops_a_u	 = []
exp_clip_res   = []
exp_clipu_res  = []

for i in range(0,NumberOfStimuli):

    a_u = random.randint(-2**20, 2**21 -1)
    a_s = random.randint(-2**20, 2**21 -1)
    
    imm = random.randint(0, 31)

    ClipLowerBound = - (2**(imm-1))
    ClipUpperBound =   (2**(imm-1) -1)

    imm_name  = 'CLIP_IMM_' + str(i)
 
    write_define(f, imm_name ,imm)

    ops_a_s.append(a_s)
    ops_a_u.append(a_u)

    if(a_s >= ClipUpperBound):
    	clip = ClipUpperBound
    elif(a_s <= ClipLowerBound):
    	clip = ClipLowerBound
    else:
        clip = a_s

    if(a_u >= ClipUpperBound):
        clipu = ClipUpperBound
    elif(a_u < 0):
        clipu = 0
    else:
        clipu = a_u

    
    exp_clip_res.append((clip) & 0xFFFFFFFF)
    exp_clipu_res.append((clipu) & 0xFFFFFFFF)


write_hex32_arr(f, 'op_a_clip' , ops_a_s)
write_hex32_arr(f, 'op_a_clipu' , ops_a_u)
write_hex32_arr(f, 'res_clip'     , exp_clip_res)
write_hex32_arr(f, 'res_clipu'    , exp_clipu_res)

ops_a_s  = []
ops_a_u  = []
ops_b_s  = []
ops_b_u  = []
exp_clip_res   = []
exp_clipu_res  = []

for i in range(0,NumberOfStimuli):

    a_u = random.randint(-2**20, 2**21 -1)
    a_s = random.randint(-2**20, 2**21 -1)
    b_u = random.randint(-2**20, 2**21 -1)
    b_s = random.randint(-2**20, 2**21 -1)

    if(a_s >= b_s):
        clip = b_s
    elif(a_s <= -(b_s +1)):
        clip = -(b_s +1)
    else:
        clip = a_s

    if(a_u >= b_u):
        clipu = b_u
    elif(a_u < 0):
        clipu = 0
    else:
        clipu = a_u

    ops_a_s.append(a_s)
    ops_a_u.append(a_u)
    ops_b_s.append(b_s)
    ops_b_u.append(b_u)

    exp_clip_res.append((clip) & 0xFFFFFFFF)
    exp_clipu_res.append((clipu) & 0xFFFFFFFF)


write_hex32_arr(f, 'op_a_clip_reg' , ops_a_s)
write_hex32_arr(f, 'op_b_clip_reg' , ops_b_s)
write_hex32_arr(f, 'res_clip_reg'     , exp_clip_res)
write_hex32_arr(f, 'op_a_clipu_reg' , ops_a_u)
write_hex32_arr(f, 'op_b_clipu_reg' , ops_b_u)
write_hex32_arr(f, 'res_clipu_reg'    , exp_clipu_res)
