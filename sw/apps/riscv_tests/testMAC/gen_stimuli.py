#!/usr/bin/env python

import sys
import random

from functools import partial


def write_hex32_arr(f, name, arr):
    f.write('unsigned int %s[] = {\n' % name)
    for v in arr:
        f.write('0x%08X,\n' % (v & 0xFFFFFFFF))
    f.write('};\n\n')
    return


N = 15

################################################################################
# instructions with two read registers
################################################################################

def instr_rr_base(prefix, func, mina, maxa, minb, maxb):
    global f

    ops_a    = []
    ops_b    = []
    exp_res  = []

    for i in range(N):
        a = random.randint(mina, maxa)
        b = random.randint(minb, maxb)

        r = func(a, b)

        ops_a.append(a)
        ops_b.append(b)
        exp_res.append(r)

    write_hex32_arr(f, '%s_a'   % prefix, ops_a)
    write_hex32_arr(f, '%s_b'   % prefix, ops_b)
    write_hex32_arr(f, '%s_exp' % prefix, exp_res)

def instr_rr(prefix, func):
    instr_rr_base(prefix, func, -2**31, 2**31-1, -2**31, 2**31-1)


################################################################################
# MAC instructions with three registers
################################################################################

def mac_base(prefix, func, mina, maxa, minb, maxb, minc, maxc):
    global f

    ops_a    = []
    ops_b    = []
    ops_c    = []
    exp_res  = []

    for i in range(N):
        a = random.randint(mina, maxa)
        b = random.randint(minb, maxb)
        c = random.randint(minc, maxc)

        r = func(a, b, c)

        ops_a.append(a)
        ops_b.append(b)
        ops_c.append(c)
        exp_res.append(r)

    write_hex32_arr(f, '%s_a'   % prefix, ops_a)
    write_hex32_arr(f, '%s_b'   % prefix, ops_b)
    write_hex32_arr(f, '%s_c'   % prefix, ops_c)
    write_hex32_arr(f, '%s_exp' % prefix, exp_res)


def mac_rrr(prefix, func):
    mac_base(prefix, func, -2**31, 2**31-1, -2**31, 2**31-1, -2**31, 2**31-1)

def mac_rrr_subw(prefix, a, b):
    v_min = {
        'zl': 0,
        'zh': 2**16,
        'sl': -2**15,
        'sh': -2**31,
    }
    v_max = {
        'zl': 2**16-1,
        'zh': 2**32-1,
        'sl': 2**15-1,
        'sh': 2**31-1,
    }
    subw_func = {
        'zl': lambda x: x,
        'zh': lambda x: (x >> 16),
        'sl': lambda x: x,
        'sh': lambda x: (x >> 16),
    }
    c_min = -2**31
    c_max = 2**31-1

    prefix = "{0}_{1}_{2}".format(prefix, a, b)
    func = lambda fa, fb, a, b, c: c + fa(a) * fb(b)
    mac_base(prefix, partial(func, subw_func[a], subw_func[b]), v_min[a], v_max[a], v_min[b], v_max[b], c_min, c_max)


################################################################################

f = open('testMAC_stimuli.h', 'w')


################################################################################
# generate testdata for regular multiplication and p.mac (three registers)
################################################################################

instr_rr('g_mul', lambda a, b: a * b)
mac_rrr('g_mac',  lambda a, b, c: (c + a * b))


################################################################################
# generate testdata for p.mac with subword selection
################################################################################

mac_rrr_subw('g_mac', 'zl', 'zl')
mac_rrr_subw('g_mac', 'zl', 'zh')
mac_rrr_subw('g_mac', 'zh', 'zl')
mac_rrr_subw('g_mac', 'zh', 'zh')

mac_rrr_subw('g_mac', 'zl', 'sl')
mac_rrr_subw('g_mac', 'zl', 'sh')
mac_rrr_subw('g_mac', 'zh', 'sl')
mac_rrr_subw('g_mac', 'zh', 'sh')

mac_rrr_subw('g_mac', 'sl', 'zl')
mac_rrr_subw('g_mac', 'sl', 'zh')
mac_rrr_subw('g_mac', 'sh', 'zl')
mac_rrr_subw('g_mac', 'sh', 'zh')

mac_rrr_subw('g_mac', 'sl', 'sl')
mac_rrr_subw('g_mac', 'sl', 'sh')
mac_rrr_subw('g_mac', 'sh', 'sl')
mac_rrr_subw('g_mac', 'sh', 'sh')

