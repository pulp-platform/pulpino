#!/usr/bin/env python-2.5

import sys
import random

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
# generate testdata for UNSIGNED right shift l.addNr{R}u 
################################################################################

ops_a    = []
ops_b    = []
ops_imm  = []
expNr_res  = []
expNrR_res  = []

imm = random.randint(0, 31)
imm_name = 'g_addNru_IMM'

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

    #r = (a % 0x100000000) >> b

    ops_a.append(a)
    ops_b.append(b)
    ops_imm.append(imm)
    expNr_res.append(rNr)
    expNrR_res.append(rNrR)
    

write_hex32_arr(f, 'g_addNru_a', ops_a)
write_hex32_arr(f, 'g_addNru_b', ops_b)
write_hex32_arr(f, 'g_addNru_imm', ops_imm)
write_hex32_arr(f, 'g_addNru_exp', expNr_res)
write_hex32_arr(f, 'g_addNrRu_exp', expNrR_res)

################################################################################
# generate testdata for SIGNED right shift l.addNr{R}
################################################################################

ops_a    = []
ops_b    = []
ops_imm  = []
expNr_res  = []
expNrR_res  = []

imm = random.randint(0, 31)
imm_name = 'g_addNr_IMM'

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
    expNr_res.append(rNr)
    expNrR_res.append(rNrR)
    

write_hex32_arr(f, 'g_addNr_a', ops_a)
write_hex32_arr(f, 'g_addNr_b', ops_b)
write_hex32_arr(f, 'g_addNr_imm', ops_imm)
write_hex32_arr(f, 'g_addNr_exp', expNr_res)
write_hex32_arr(f, 'g_addNrR_exp', expNrR_res)


################################################################################
# generate testdata for left shift l.addNl{R}
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
# generate testdata for SIGNED right shift l.subNr{R}
################################################################################

ops_a    = []
ops_b    = []
ops_imm  = []
expNr_res  = []
expNrR_res  = []

imm = random.randint(0, 31)
imm_name = 'g_subNr_IMM'

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
    expNr_res.append(rNr)
    expNrR_res.append(rNrR)
    

write_hex32_arr(f, 'g_subNr_a', ops_a)
write_hex32_arr(f, 'g_subNr_b', ops_b)
write_hex32_arr(f, 'g_subNr_imm', ops_imm)
write_hex32_arr(f, 'g_subNr_exp', expNr_res)
write_hex32_arr(f, 'g_subNrR_exp', expNrR_res)


################################################################################
# generate testdata for left shift l.subNl{R}
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



