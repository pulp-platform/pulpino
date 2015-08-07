#!/usr/bin/python

# ////////////////////////////////////////////////////////////////////////////////
# // Company:        Multitherman Laboratory @ DEIS - University of Bologna     //
# //                    Viale Risorgimento 2 40136                              //
# //                    Bologna - fax 0512093785 -                              //
# //                                                                            //
# // Engineer:       Davide Rossi - davide.rossi@unibo.it                       //
# //                                                                            //
# // Additional contributions by:                                               //
# //                 Andreas Traber - atraber@student.ethz.ch                   //
# //                 Michael Gautschi - gautschi@iis.ethz.ch                    //
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
# // Revision v0.5 - Rewrote the whole thing in python to be consistent with    //
# //                 s19toslm                                                   //
# ////////////////////////////////////////////////////////////////////////////////

import sys
import math


if(len(sys.argv) < 2):
    print "Usage s19toboot.py FILENAME"
    quit()


rom_size      = 512 # in double words (64 bit)
rom_start     = 0x1A000000
rom_end       = rom_start + rom_size * 8 - 1


###############################################################################
# Parse s19 file
###############################################################################
s19_file = open(sys.argv[1], 'r')
s19_dict = {}

for line in s19_file:
    rec_field = line[:1]
    prefix    = line[:3]

    if rec_field == "S0" or prefix == "S009" or prefix == "S505" or prefix == "S705" or prefix == "S017" or line == "":
        continue

    addr = int("0x%s" % line[4:12], 0)
    data = line[12:14]

    s19_dict[addr] = data

s19_file.close()

slm_dict = {}

for addr in s19_dict:
    wordaddr = addr >> 2
    data = "00000000"

    if wordaddr in slm_dict:
        data = slm_dict[wordaddr]

    byte = addr % 4
    byte0 = data[0:2]
    byte1 = data[2:4]
    byte2 = data[4:6]
    byte3 = data[6:8]
    new   = s19_dict[addr]

    if byte == 0:
        data = "%s%s%s%s" % (byte0, byte1, byte2, new)
    elif byte == 1:
        data = "%s%s%s%s" % (byte0, byte1, new,   byte3)
    elif byte == 2:
        data = "%s%s%s%s" % (byte0, new,   byte2, byte3)
    elif byte == 3:
        data = "%s%s%s%s" % (new,   byte1, byte2, byte3)

    slm_dict[wordaddr] = data



# word align all addresses
rom_start   = rom_start   >> 2
rom_end     = rom_end     >> 2

###############################################################################
# open files
###############################################################################
rom_file = open("boot_code.cde" , 'w')


###############################################################################
# write the stimuli
###############################################################################
for addr in sorted(slm_dict.keys()):
    data = slm_dict[addr]

    # l2 address range
    if(addr >= rom_start and addr <= rom_end):
        rom_base = addr - rom_start
        rom_addr = (rom_base >> 1)

        if((addr%2) == 0):
            data_even = data
        else:
            data_odd  = data
            rom_file.write("%s%s\n" % (data_odd, data_even))

###############################################################################
# close all files
###############################################################################
rom_file.close()
