#!/usr/bin/env python

import csv
import sys,os

def readSLM(fn, baseaddr, offset):
    with open(fn) as f:
        slmreader = csv.reader(f, delimiter=' ')
        mwr_phys_ls = []
        for row in slmreader:
            mwr_phys = {}
            try:
                mwr_phys['addr'] = int(row[0].strip('@'), base=16)*4*2 + offset +baseaddr
                mwr_phys['data'] = int(row[1].strip('@'), base=16)
                mwr_phys_ls.append(mwr_phys)
            except IndexError:
                pass
    return mwr_phys_ls

def slm2tcl(mwr_phys_lo0, mwr_phys_hi0, mwr_phys_lo1, mwr_phys_hi1):
    string = ""
    for i in xrange(0,len(mwr_phys_hi0)):
        l = mwr_phys_lo0[i]
        h = mwr_phys_hi0[i]
        string = string + "mwr_phys 0x%08x 0x%08x\n" % (l['addr'], l['data'])
        string = string + "mwr_phys 0x%08x 0x%08x\n" % (h['addr'], h['data'])
    # for l,h in zip(mwr_phys_lo1, mwr_phys_hi1):
    for i in xrange(0,len(mwr_phys_hi1)):
        l = mwr_phys_lo1[i]
        h = mwr_phys_hi1[i]
        string = string + "mwr_phys 0x%08x 0x%08x\n" % (l['addr'], l['data'])
        string = string + "mwr_phys 0x%08x 0x%08x\n" % (h['addr'], h['data'])
    string = string + "\n"
    return string

if __name__=="__main__":
    if len(sys.argv) < 6:
        sys.exit('Usage: %s l2_lo0.slm l2_hi0.slm l2_lo1.slm l2_hi1.slm output_file.tcl [[0x]DEADBEEF]' % sys.argv[0])
    
    if not os.path.exists(sys.argv[1]):
        sys.exit('Error: file %s was not found' % sys.argv[1])
    if not os.path.exists(sys.argv[2]):
        sys.exit('Error: file %s was not found' % sys.argv[2])
    if not os.path.exists(sys.argv[3]):
        sys.exit('Error: file %s was not found' % sys.argv[3])
    if not os.path.exists(sys.argv[4]):
        sys.exit('Error: file %s was not found' % sys.argv[4])
    
    if len(sys.argv) == 7:
        if sys.argv[6][1] == 'x' or sys.argv[3][1] == 'X':
            baseaddr_str = sys.argv[6][2:]
        else:
            baseaddr_str = sys.argv[6]
        baseaddr = int(baseaddr_str, base=16)
    else:
        baseaddr = 0x0
    
    mwr_phys_lo0 = readSLM(sys.argv[1], baseaddr, 0x0)
    mwr_phys_hi0 = readSLM(sys.argv[2], baseaddr, 0x4)
    mwr_phys_lo1 = readSLM(sys.argv[3], baseaddr, 0x4000)
    mwr_phys_hi1 = readSLM(sys.argv[4], baseaddr, 0x4004)

    mwr_phys_str = slm2tcl(mwr_phys_lo0, mwr_phys_hi0, mwr_phys_lo1, mwr_phys_hi1)

    with open(sys.argv[5], 'w') as f:
        f.write(mwr_phys_str)

