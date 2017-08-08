#!/usr/bin/env python

# Copyright 2017 ETH Zurich and University of Bologna.
# Copyright and related rights are licensed under the Solderpad Hardware
# License, Version 0.51 (the License); you may not use this file except in
# compliance with the License.  You may obtain a copy of the License at
# http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
# or agreed to in writing, software, hardware and materials distributed under
# this License is distributed on an AS IS BASIS, WITHOUT WARRANTIES OR
# CONDITIONS OF ANY KIND, either express or implied. See the License for the
# specific language governing permissions and limitations under the License.

# ////////////////////////////////////////////////////////////////////////////////
# // Company:        Multitherman Laboratory @ DEIS - University of Bologna     //
# //                    Viale Risorgimento 2 40136                              //
# //                    Bologna - fax 0512093785 -                              //
# //                                                                            //
# // Engineer:       Davide Rossi - davide.rossi@unibo.it                       //
# //                                                                            //
# // Additional contributions by:                                               //
# //                 Andreas Traber - atraber@student.ethz.ch                   //
# //                                                                            //
# // Create Date:    05/04/2013                                                 //
# // Design Name:    ULPSoC                                                     //
# // Project Name:   ULPSoC                                                     //
# // Language:       tcl, now python                                            //
# //                                                                            //
# // Description:    s19 to slm conversion tool for stxp70 cluster compilation  //
# //                                                                            //
# // Revision:                                                                  //
# // Revision v0.1 - File Created                                               //
# // Revision v0.2 - Modification: Compiler does now generate little endian     //
# //                 directly. revert bytes!                                    //
# // Revision v0.3 - Moved from 128 bit s19 to 8 bit s19 file. This solves our  //
# //                 problems with misaligned addresses in s19 files.           //
# // Revision v0.4 - Added TCDM memory initialization                           //
# // Revision v0.5 - Rewrote the whole thing in python as tcl cannot handle     //
# //                 long file names properly
# ////////////////////////////////////////////////////////////////////////////////

import sys
import math


###############################################################################
# Function to dump single bytes of a string to a file
###############################################################################
def dump_bytes( filetoprint, addr, data_s):
    for i in xrange(0,4,1):
        filetoprint.write("@%08X %s\n" % ( addr+i,  data_s[i*2:(i+1)*2] ))

###############################################################################
# Read s19 file and put data bytes into a dictionary
###############################################################################
def s19_parse(filename, s19_dict):
    s19_file = open(filename, 'r')
    for line in s19_file:
        rec_field = line[:2]
        prefix    = line[:4]

        if rec_field == "S0" or prefix == "S009" or prefix == "S505" or prefix == "S705" or prefix == "S017" or prefix == "S804" or line == "":
            continue

        data = line[-6:-4] # extract data byte
        str_addr = line[4:-6]

        addr = int("0x%s" % str_addr, 0)


        s19_dict[addr] = data

    s19_file.close()

###############################################################################
# arrange bytes in words
###############################################################################
def bytes_to_words(byte_dict, word_dict):
    for addr in byte_dict:
        wordaddr = addr >> 2
        data = "00000000"

        if wordaddr in word_dict:
            data = word_dict[wordaddr]

        byte = addr % 4
        byte0 = data[0:2]
        byte1 = data[2:4]
        byte2 = data[4:6]
        byte3 = data[6:8]
        new   = byte_dict[addr]

        if byte == 0:
            data = "%s%s%s%s" % (byte0, byte1, byte2, new)
        elif byte == 1:
            data = "%s%s%s%s" % (byte0, byte1, new,   byte3)
        elif byte == 2:
            data = "%s%s%s%s" % (byte0, new,   byte2, byte3)
        elif byte == 3:
            data = "%s%s%s%s" % (new,   byte1, byte2, byte3)

        word_dict[wordaddr] = data

###############################################################################
# Start of file
###############################################################################
if(len(sys.argv) < 2):
    print "Usage s19toslm.py FILENAME"
    quit()


l2_banks     = 1
l2_bank_size = 8192 # in words (32 bit)
l2_start     = 0x00000000
l2_end       = l2_start + l2_banks * l2_bank_size * 4 - 1

tcdm_banks     = 1
tcdm_bank_size = 6144 # in words (32 bit)
tcdm_start     = 0x00100000
tcdm_end       = tcdm_start + tcdm_banks * tcdm_bank_size * 4 - 1
tcdm_bank_bits = int(math.log(tcdm_banks, 2))


###############################################################################
# Parse s19 file
###############################################################################
s19_dict = {}
slm_dict = {}

s19_parse(sys.argv[1], s19_dict)

bytes_to_words(s19_dict, slm_dict)


# word align all addresses
l2_start   = l2_start   >> 2
l2_end     = l2_end     >> 2
tcdm_start = tcdm_start >> 2
tcdm_end   = tcdm_end   >> 2
# actual size of L2 and TCDM (for flash)
l2_size     = 0
tcdm_size   = 0
l2_blocks   = 0
tcdm_blocks = 0
###############################################################################
# open files
###############################################################################
l2_cut_files = {}
for i in range(0, l2_banks):
    l2_cut_files[i] = {}
    l2_cut_files[i]['lo'] = open("l2_ram_cut%d_lo.slm" % i, 'w')
    l2_cut_files[i]['hi'] = open("l2_ram_cut%d_hi.slm" % i, 'w')

tcdm_files = {}
for i in range(0, tcdm_banks):
    tcdm_files[i] = open("tcdm_bank%d.slm" % i, 'w')


spi_stim = open("spi_stim.txt",   'w')
l2_stim  = open("l2_stim.slm",    'w')
flash    = open("flash_stim.slm", 'w')

###############################################################################
# write the stimuli
###############################################################################
for addr in sorted(slm_dict.keys()):
    data = slm_dict[addr]

    # l2 address range
    if(addr >= l2_start and addr <= l2_end):
        l2_base = addr - l2_start
        cut     = l2_base >> (int(math.log(l2_bank_size, 2) + 1))
        hilo    = 'hi' if (addr & 0x1) else 'lo'
        l2_addr = (l2_base >> 1) % l2_bank_size
        l2_size += 1

        if addr & 0x4:
            l2_cut_files[cut][hilo].write("@%08X %s\n" % (l2_addr, data))
        else:
            l2_cut_files[cut][hilo].write("@%08X %s\n" % (l2_addr, data))

        l2_stim.write("@%08X %s\n" % (l2_base, data))
        spi_stim.write("%08X_%s\n" % (addr << 2, data))


    # tcdm address range
    if(addr >= tcdm_start and addr <= tcdm_end):
        tcdm_addr = (addr - tcdm_start) >> tcdm_bank_bits
        bank      = addr % tcdm_banks
        tcdm_files[bank].write("@%08X %s\n" % (tcdm_addr, data))
        tcdm_size += 1

        spi_stim.write("%08X_%s\n" % (addr << 2, data))
###############################################################################
# write flash
###############################################################################

# 4KB blocks
l2_blocks   = (l2_size/1024+1)
tcdm_blocks = (tcdm_size/1024+1)
header_size = 8<<2

l2_off_s    = "%08X"%(((tcdm_size+8)/1024 + 1)*1024 <<2)
l2_start_s  = "%08X"%(l2_start << 2)
l2_size_s   = "%08X"%(l2_size << 2)
l2_blocks_s = "%08X"%(l2_blocks)

tcdm_off_s    = "%08X"%(header_size)
tcdm_start_s  = "%08X"%(tcdm_start << 2)
tcdm_size_s   = "%08X"%(tcdm_size << 2)
tcdm_blocks_s = "%08X"%(tcdm_blocks)


# write header
dump_bytes(flash, 0,  l2_off_s)
dump_bytes(flash, 4,  l2_start_s)
dump_bytes(flash, 8,  l2_size_s)
dump_bytes(flash, 12, l2_blocks_s)
dump_bytes(flash, 16, tcdm_off_s)
dump_bytes(flash, 20, tcdm_start_s)
dump_bytes(flash, 24, tcdm_size_s)
dump_bytes(flash, 28, tcdm_blocks_s)

# write data
for addr in sorted(slm_dict.keys()):
    data = slm_dict[addr]

    # l2 address range
    if(addr >= l2_start and addr <= l2_end):
        l2_base = (addr - l2_start)
        l2_addr = l2_base  + ((tcdm_size+8)/1024+1)*1024
        dump_bytes(flash, l2_addr * 4, data)

    # tcdm address range
    if(addr >= tcdm_start and addr <= tcdm_end):
        tcdm_addr = (addr - tcdm_start)

        dump_bytes(flash, tcdm_addr * 4 + header_size, data)

###############################################################################
# close all files
###############################################################################
for i in l2_cut_files:
    l2_cut_files[i]['hi'].close()
    l2_cut_files[i]['lo'].close()

for i in tcdm_files:
    tcdm_files[i].close()

spi_stim.close()
l2_stim.close()
flash.close()
