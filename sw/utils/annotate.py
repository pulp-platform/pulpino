#!/usr/bin/env python

import sys

# Copyright 2017 ETH Zurich and University of Bologna.
# Copyright and related rights are licensed under the Solderpad Hardware
# License, Version 0.51 (the License); you may not use this file except in
# compliance with the License.  You may obtain a copy of the License at
# http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
# or agreed to in writing, software, hardware and materials distributed under
# this License is distributed on an AS IS BASIS, WITHOUT WARRANTIES OR
# CONDITIONS OF ANY KIND, either express or implied. See the License for the
# specific language governing permissions and limitations under the License.

class Section:
    """Encodes a Section"""
    def __init__(self, start, end, name):
        self.start = start
        self.end = end
        self.name = name
    def contains(self, pc):
        if self.start <= pc and pc < self.end:
            return True;
        else:
            return False;

f = open('trace_core_00.log', 'r')
a = open(sys.argv[1], 'r')
r = open('trace_core_00_annotated.log', 'w')


save = []
name = []
first = True
sections = []

# parse disassembly and extract sections
for line in a:
    if line.startswith("0"):
        save.append(int("0x" + line.split()[0], 16))
        name.append(line.split()[1])

save.append(1000)

# save sections to section objects
for i in range(len(name)):
        sections.append(Section(save[i], save[i+1], name[i]))

sections[len(sections) - 1].end =  sections[len(sections) - 1].start + 1000

for i in range(len(sections)):
    print(sections[i].name + " " + str(sections[i].start) + ", " + str(sections[i].end) + "\n")

# annotate file
first = True
for line in f:
    if first:
        r.write(line + "\n")
        first = False
    else:
        # r.write(line)
        try:
            pc = int(line.split()[3], 16)
            # print(str(pc);
            for i in range(len(sections)):
                if sections[i].contains(pc):
                    r.write(line.rstrip() + "\t" + sections[i].name + "\n")

        except ValueError:
            r.write("Value Error")
            pass



