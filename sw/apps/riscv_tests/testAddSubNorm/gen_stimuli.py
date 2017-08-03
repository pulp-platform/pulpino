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


f = open('testAddSubNorm_stimuli.h', 'w')


NumberOfStimuli = 10

write_define(f, 'NumberOfStimuli',NumberOfStimuli)

################################################################################
################################################################################
#
# ADDITION
#
################################################################################
################################################################################

################################################################################
# generate testdata for UNSIGNED right shift p.addu[R]N[r]
################################################################################

ops_a    = []
ops_b    = []
ops_imm  = []
ops_ar   = []
ops_br   = []
ops_cr   = []
expN_res   = []
expRN_res  = []
expNr_res  = []
expRNr_res = []

imm = random.randint(0, 31)
imm_name = 'g_adduRN_IMM'

write_define(f, imm_name ,imm)

  
for i in range(0,NumberOfStimuli):
    a = random.randint(0, 2**32-1)
    b = random.randint(0, 2**32-1)
     
    if(imm == 0):
        rnd = 0
    else:
        rnd = 2**(imm-1)

    rNr  = (a + b) & 0xFFFFFFFF
    
    rNr  = (rNr >> imm) & 0xFFFFFFFF
    
    rNrR  = (a + b + rnd) & 0xFFFFFFFF
    
    rNrR  = (rNrR >> imm) & 0xFFFFFFFF

    ops_a.append(a)
    ops_b.append(b)
    ops_imm.append(imm)
    expN_res.append(rNr)
    expRN_res.append(rNrR)

    #register version, only for riscv
    c = random.randint(-2**31, 2**31-1)
    a = random.randint(-2**31, 2**31-1)
    b = random.randint(0, 31)

    if(b == 0):
        rnd = 0
    else:
        rnd = 2**(b-1)

    rNr  = (c + a) & 0xFFFFFFFF

    rNr  = (rNr >> b) & 0xFFFFFFFF

    rNrR  = (c + a + rnd) & 0xFFFFFFFF

    rNrR  = (rNrR >> b) & 0xFFFFFFFF

    ops_ar.append(a)
    ops_br.append(b)
    ops_cr.append(c)
    expNr_res.append(rNr)
    expRNr_res.append(rNrR)

write_hex32_arr(f, 'g_adduRN_a', ops_a)
write_hex32_arr(f, 'g_adduRN_b', ops_b)
write_hex32_arr(f, 'g_adduRN_imm', ops_imm)
write_hex32_arr(f, 'g_adduN_exp', expN_res)
write_hex32_arr(f, 'g_adduRN_exp', expRN_res)
write_hex32_arr(f, 'g_adduRNr_a', ops_ar)
write_hex32_arr(f, 'g_adduRNr_b', ops_br)
write_hex32_arr(f, 'g_adduRNr_c', ops_cr)
write_hex32_arr(f, 'g_adduNr_exp', expNr_res)
write_hex32_arr(f, 'g_adduRNr_exp', expRNr_res)

################################################################################
# generate testdata for SIGNED right shift p.add[R]N[r]
################################################################################

ops_a    = []
ops_b    = []
ops_imm  = []
ops_ar   = []
ops_br   = []
ops_cr   = []
expN_res   = []
expRN_res  = []
expNr_res  = []
expRNr_res = []

imm = random.randint(0, 31)
imm_name = 'g_addRN_IMM'

write_define(f, imm_name ,imm)

for i in range(0,NumberOfStimuli):
    a = random.randint(-2**31, 2**31-1)
    b = random.randint(-2**31, 2**31-1)

    if(imm == 0):
        rnd = 0
    else:
        rnd = 2**(imm-1)

    rNr  = (a + b) & 0xFFFFFFFF  
    if(rNr & (1<<31)): #signed
        rNr = rNr | 0xFFFFFFFF00000000

    rNr  = (rNr >> imm) & 0xFFFFFFFF 
    
    rNrR  = (a + b + rnd) & 0xFFFFFFFF  
    if(rNrR & (1<<31)): #signed
        rNrR = rNrR | 0xFFFFFFFF00000000

    rNrR  = (rNrR >> imm) & 0xFFFFFFFF 
    

    ops_a.append(a)
    ops_b.append(b)
    ops_imm.append(imm)
    expN_res.append(rNr)
    expRN_res.append(rNrR)

    #register version, only for riscv
    c = random.randint(-2**31, 2**31-1)
    a = random.randint(-2**31, 2**31-1)
    b = random.randint(0, 31)

    if(b == 0):
        rnd = 0
    else:
        rnd = 2**(b-1)

    rNr  = (c + a) & 0xFFFFFFFF  
    if(rNr & (1<<31)): #signed
        rNr = rNr | 0xFFFFFFFF00000000

    rNr  = (rNr >> b) & 0xFFFFFFFF 
    
    rNrR  = (c + a + rnd) & 0xFFFFFFFF  
    if(rNrR & (1<<31)): #signed
        rNrR = rNrR | 0xFFFFFFFF00000000

    rNrR  = (rNrR >> b) & 0xFFFFFFFF 

    ops_ar.append(a)
    ops_br.append(b)
    ops_cr.append(c)
    expNr_res.append(rNr)
    expRNr_res.append(rNrR)

write_hex32_arr(f, 'g_addRN_a', ops_a)
write_hex32_arr(f, 'g_addRN_b', ops_b)
write_hex32_arr(f, 'g_addRN_imm', ops_imm)
write_hex32_arr(f, 'g_addN_exp', expN_res)
write_hex32_arr(f, 'g_addRN_exp', expRN_res)
write_hex32_arr(f, 'g_addRNr_a', ops_ar)
write_hex32_arr(f, 'g_addRNr_b', ops_br)
write_hex32_arr(f, 'g_addRNr_c', ops_cr)
write_hex32_arr(f, 'g_addNr_exp', expNr_res)
write_hex32_arr(f, 'g_addRNr_exp', expRNr_res)


################################################################################
# generate testdata for left shift l.addNl{R}, for orion only
################################################################################

ops_a    = []
ops_b    = []
ops_imm  = []
expNl_res  = []
expNlR_res  = []

imm = random.randint(0, 31)
imm_name = 'g_addNl_IMM'

write_define(f, imm_name ,imm)
for i in range(0,NumberOfStimuli):
    a = random.randint(-2**31, 2**31-1)
    b = random.randint(-2**31, 2**31-1)



    if(imm == 0):
        rnd = 0
    else:
        rnd = 2**(imm-1)

    rNl  = (a + b) << imm

    ops_a.append(a)
    ops_b.append(b)
    ops_imm.append(imm)
    expNl_res.append(rNl)   

write_hex32_arr(f, 'g_addNl_a', ops_a)
write_hex32_arr(f, 'g_addNl_b', ops_b)
write_hex32_arr(f, 'g_addNl_imm', ops_imm)
write_hex32_arr(f, 'g_addNl_exp', expNl_res)



################################################################################
################################################################################
#
# SUBTRACTION
#
################################################################################
################################################################################

################################################################################
# generate testdata for UNSIGNED right shift p.subu[R]N[r]
################################################################################

ops_a    = []
ops_b    = []
ops_imm  = []
ops_ar   = []
ops_br   = []
ops_cr   = []
expN_res   = []
expRN_res  = []
expNr_res  = []
expRNr_res = []

imm = random.randint(0, 31)
imm_name = 'g_subuRN_IMM'

write_define(f, imm_name ,imm)
for i in range(0,NumberOfStimuli):
    a = random.randint(-2**31, 2**31-1)
    b = random.randint(-2**31, 2**31-1)



    if(imm == 0):
        rnd = 0
    else:
        rnd = 2**(imm-1)
    
    rNr  = (a - b) & 0xFFFFFFFF

    rNr  = (rNr >> imm) & 0xFFFFFFFF

    rNrR  = (a - b + rnd) & 0xFFFFFFFF

    rNrR  = (rNrR >> imm) & 0xFFFFFFFF

    ops_a.append(a)
    ops_b.append(b)
    ops_imm.append(imm)
    expN_res.append(rNr)
    expRN_res.append(rNrR)

    #register version, only for riscv
    c = random.randint(-2**31, 2**31-1)
    a = random.randint(-2**31, 2**31-1)
    b = random.randint(0, 31)

    if(b == 0):
        rnd = 0
    else:
        rnd = 2**(b-1)

    rNr  = (c - a) & 0xFFFFFFFF
    rNr  = (rNr >> b) & 0xFFFFFFFF

    rNrR  = (c - a + rnd) & 0xFFFFFFFF
    rNrR  = (rNrR >> b) & 0xFFFFFFFF

    ops_ar.append(a)
    ops_br.append(b)
    ops_cr.append(c)
    expNr_res.append(rNr)
    expRNr_res.append(rNrR)


write_hex32_arr(f, 'g_subuRN_a', ops_a)
write_hex32_arr(f, 'g_subuRN_b', ops_b)
write_hex32_arr(f, 'g_subuRN_imm', ops_imm)
write_hex32_arr(f, 'g_subuN_exp', expN_res)
write_hex32_arr(f, 'g_subuRN_exp', expRN_res)
write_hex32_arr(f, 'g_subuRNr_a', ops_ar)
write_hex32_arr(f, 'g_subuRNr_b', ops_br)
write_hex32_arr(f, 'g_subuRNr_c', ops_cr)
write_hex32_arr(f, 'g_subuNr_exp', expNr_res)
write_hex32_arr(f, 'g_subuRNr_exp', expRNr_res)

################################################################################
# generate testdata for SIGNED right shift p.sub[R]N[r]
################################################################################

ops_a    = []
ops_b    = []
ops_imm  = []
ops_ar   = []
ops_br   = []
ops_cr   = []
expN_res   = []
expRN_res  = []
expNr_res  = []
expRNr_res = []

imm = random.randint(0, 31)
imm_name = 'g_subRN_IMM'

write_define(f, imm_name ,imm)
for i in range(0,NumberOfStimuli):
    a = random.randint(-2**31, 2**31-1)
    b = random.randint(-2**31, 2**31-1)



    if(imm == 0):
        rnd = 0
    else:
        rnd = 2**(imm-1)
    
    rNr  = (a - b) & 0xFFFFFFFF  
    if(rNr & (1<<31)): #signed
        rNr = rNr | 0xFFFFFFFF00000000

    rNr  = (rNr >> imm) & 0xFFFFFFFF 
    
    rNrR  = (a - b + rnd) & 0xFFFFFFFF  
    if(rNrR & (1<<31)): #signed
        rNrR = rNrR | 0xFFFFFFFF00000000

    rNrR  = (rNrR >> imm) & 0xFFFFFFFF 

    ops_a.append(a)
    ops_b.append(b)
    ops_imm.append(imm)
    expN_res.append(rNr)
    expRN_res.append(rNrR)

    #register version, only for riscv
    c = random.randint(-2**31, 2**31-1)
    a = random.randint(-2**31, 2**31-1)
    b = random.randint(0, 31)

    if(b == 0):
        rnd = 0
    else:
        rnd = 2**(b-1)

    rNr  = (c - a) & 0xFFFFFFFF  
    if(rNr & (1<<31)): #signed
        rNr = rNr | 0xFFFFFFFF00000000

    rNr  = (rNr >> b) & 0xFFFFFFFF 
    
    rNrR  = (c - a + rnd) & 0xFFFFFFFF  
    if(rNrR & (1<<31)): #signed
        rNrR = rNrR | 0xFFFFFFFF00000000

    rNrR  = (rNrR >> b) & 0xFFFFFFFF 

    ops_ar.append(a)
    ops_br.append(b)
    ops_cr.append(c)
    expNr_res.append(rNr)
    expRNr_res.append(rNrR)


write_hex32_arr(f, 'g_subRN_a', ops_a)
write_hex32_arr(f, 'g_subRN_b', ops_b)
write_hex32_arr(f, 'g_subRN_imm', ops_imm)
write_hex32_arr(f, 'g_subN_exp', expN_res)
write_hex32_arr(f, 'g_subRN_exp', expRN_res)
write_hex32_arr(f, 'g_subRNr_a', ops_ar)
write_hex32_arr(f, 'g_subRNr_b', ops_br)
write_hex32_arr(f, 'g_subRNr_c', ops_cr)
write_hex32_arr(f, 'g_subNr_exp', expNr_res)
write_hex32_arr(f, 'g_subRNr_exp', expRNr_res)

################################################################################
# generate testdata for left shift l.subNl{R}, orion only
################################################################################

ops_a    = []
ops_b    = []
ops_imm  = []
expNl_res  = []
expNlR_res  = []
imm = random.randint(0, 31)
imm_name = 'g_subNl_IMM'

write_define(f, imm_name ,imm)
for i in range(0,NumberOfStimuli):
    a = random.randint(-2**31, 2**31-1)
    b = random.randint(-2**31, 2**31-1)



    if(imm == 0):
        rnd = 0
    else:
        rnd = 2**(imm-1)


    rNl  = (a - b) << imm

    ops_a.append(a)
    ops_b.append(b)
    ops_imm.append(imm)
    expNl_res.append(rNl)
    

write_hex32_arr(f, 'g_subNl_a', ops_a)
write_hex32_arr(f, 'g_subNl_b', ops_b)
write_hex32_arr(f, 'g_subNl_imm', ops_imm)
write_hex32_arr(f, 'g_subNl_exp', expNl_res)



