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
import os


if(len(sys.argv) < 2):
    print "Usage s19toboot.py FILENAME"
    quit()


rom_size      = 548 # in words (32 bit)
rom_start     = 0x00008000
rom_end       = rom_start + rom_size * 4 - 1


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


s19_dict = {}
slm_dict = {}

s19_parse(sys.argv[1], s19_dict)

# fill slm_dict with 0's
for wordaddr in xrange(rom_start >> 2, (rom_end>>2) + 1):
    slm_dict[wordaddr] = "00000000"

bytes_to_words(s19_dict, slm_dict)


# word align all addresses
rom_start   = rom_start   >> 2
rom_end     = rom_end     >> 2

###############################################################################
# open files
###############################################################################
rom_file  = open("boot_code.cde", 'w')
vlog_file = open("boot_code.sv",  'w')

# prepare file
vlog_file.write("""
module boot_code
(
    input  logic        CLK,
    input  logic        RSTN,

    input  logic        CSN,
    input  logic [%d:0]  A,
    output logic [31:0] Q
  );

  const logic [0:%d] [31:0] mem = {
""" % (math.log(rom_size, 2), (rom_size-1)))

###############################################################################
# write the stimuli
###############################################################################
for addr in sorted(slm_dict.keys()):
    data = slm_dict[addr]

    # l2 address range
    if(addr >= rom_start and addr <= rom_end):
        rom_base = addr - rom_start
        rom_addr = (rom_base >> 1)

        vlog_file.write("    32'h%s,\n" % (data))

# remove ,\n
vlog_file.seek(-2, os.SEEK_END)
vlog_file.write("""};

  logic [%d:0] A_Q;

  always_ff @(posedge CLK, negedge RSTN)
  begin
    if (~RSTN)
      A_Q <= '0;
    else
      if (~CSN)
        A_Q <= A;
  end

  assign Q = mem[A_Q];

endmodule""" % (math.log(rom_size, 2)));

###############################################################################
# close all files
###############################################################################
rom_file.close()
vlog_file.close()
