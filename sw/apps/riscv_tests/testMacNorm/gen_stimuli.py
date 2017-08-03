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


f = open('testMacNorm_stimuli.h', 'w')


NumberOfStimuli = 10

write_define(f, 'NumberOfStimuli',NumberOfStimuli)



def mac_base(prefix, right, signA, signB,mac,rounding):
    global f

    
    imm = random.randint(0, 31)

    imm_name = prefix + '_IMM'

    write_define(f, imm_name ,imm)


    if(imm != 0 and right==1 and rounding==1):
        rnd = 2**(imm-1)
    else:
        rnd = 0
    
    upperboundA = 2**16-1
    lowerboundA = 0
    
    upperboundB = 2**16-1
    lowerboundB = 0

    upperboundC = 2**32-1
    lowerboundC = 0
    
    if(signA):
      upperboundA = 2**15-1
      lowerboundA = -2**15
   
    if(signB):
      upperboundB = 2**15-1
      lowerboundB = -2**15

    sign = (signA | signB)
    
    if(sign):
      upperboundc = 2**31-1
      lowerboundc = -2**31


    func = lambda a, b, c, rnd: ((c + a*b + rnd) & 0xFFFFFFFF)

    ops_b    = []
    ops_a	 = []
    ops_c    = []
    exp_res  = []

    for i in range(0,NumberOfStimuli):

        a = random.randint(lowerboundA, upperboundA)
        b = random.randint(lowerboundB, upperboundB)
        
          
        if(mac == 1):
            
            c = random.randint(lowerboundC, upperboundC)
            
        else:
            
            c = 0
            


        ops_a.append(a)
        ops_b.append(b)
        
        ops_c.append(c)
        
        r = func(a, b, c,rnd)
        
        if((r & (1<<31))!=0 and right==1 and (sign==1)):
            r = r | 0xFFFFFFFF00000000 #extends sign
        


        if(right == 1):
            r = (r >> imm) & 0xFFFFFFFF
        else: 
            r = (r << imm) & 0xFFFFFFFF
     
        
        exp_res.append(r)

    
    write_hex32_arr(f, '%s_a'   % prefix, ops_a)
    write_hex32_arr(f, '%s_b'   % prefix, ops_b)
    
    if(mac):
        write_hex32_arr(f, '%s_c'   % prefix, ops_c)
    
    write_hex32_arr(f, '%s_exp' % prefix, exp_res)




#### imm1 ####

mac_base ('g_mulsNl'    , 0 , 1, 1, 0, 0)
mac_base ('g_macsNl'    , 0 , 1, 1, 1, 0)

mac_base ('g_muluNl'    , 0 , 0, 0, 0, 0)
mac_base ('g_macuNl'    , 0 , 0, 0, 1, 0)

mac_base ('g_mulusNl'   , 0 , 0, 1, 0, 0)
mac_base ('g_macusNl'   , 0 , 0, 1, 1, 0)


mac_base ('g_mulsNr'    , 1 , 1, 1, 0, 0)
mac_base ('g_macsNr'    , 1 , 1, 1, 1, 0)

mac_base ('g_muluNr'    , 1 , 0, 0, 0, 0)
mac_base ('g_macuNr'    , 1 , 0, 0, 1, 0)

mac_base ('g_mulusNr'   , 1 , 0, 1, 0, 0)
mac_base ('g_macusNr'   , 1 , 0, 1, 1, 0)

mac_base ('g_mulsNrR'   , 1 , 1, 1, 0, 1)
mac_base ('g_macsNrR'   , 1 , 1, 1, 1, 1)

mac_base ('g_muluNrR'   , 1 , 0, 0, 0, 1)
mac_base ('g_macuNrR'   , 1 , 0, 0, 1, 1)

mac_base ('g_mulusNrR'  , 1 , 0, 1, 0, 1)
mac_base ('g_macusNrR'  , 1 , 0, 1, 1, 1)

