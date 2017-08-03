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

def write_arr(f, name, arr, type, height, width):
    f.write('__attribute__ ((section(".heapsram"))) %s %s[%d][%d] = {\n' % (type, name, height, width))
    for i in range(0, height):
        f.write('{\n')
        for j in range(0, width):
            v = arr[i * height + j]
            f.write('%d, ' % (v))
        f.write('},\n')
    f.write('};\n\n')
    return

################################################################################

def gen_stimuli(name, type, min, max):
    f = open(name, 'w')
    
    
    HEIGHT = 32
    WIDTH  = 32
    
    m_a   = []
    m_b   = []
    m_exp = []
    
    for i in range(0,HEIGHT):
        for j in range(0,WIDTH):
            a = random.randint(min, max)
            b = random.randint(min, max)
    
            r = (a + b)
    
            m_a.append(a)
            m_b.append(b)
            m_exp.append(r)
    
    write_arr(f, 'm_a',   m_a,   type, HEIGHT, WIDTH)
    write_arr(f, 'm_b',   m_b,   type, HEIGHT, WIDTH)
    write_arr(f, 'm_c',   m_a,   type, HEIGHT, WIDTH)
    write_arr(f, 'm_exp', m_exp, type, HEIGHT, WIDTH)
    
    f.write('#define WIDTH %d\n' % HEIGHT)
    f.write('#define HEIGHT %d\n'% WIDTH)

gen_stimuli('matrixAdd32.h', "int32_t", -2**30, 2**30-1)
gen_stimuli('matrixAdd16.h', "int16_t", -2**14, 2**14-1)
gen_stimuli('matrixAdd8.h',  "int8_t",  -2**6, 2**6-1)
