#!/usr/bin/python

import sys

class Section:
    """Encodes a Section"""
    def __init__(self, start, end, name):
        self.start = start
        self.end = end
        self.name = name
    def containing(self, pc):
        if self.start <= pc and pc < self.end:
            return True;
        else:
            return False;

# a = open('helloworld.read', 'r')

f = open('trace_core_00.log', 'r')
a = open(sys.argv[1], 'r')
r = open('trace_core_00_annotated.log', 'w')


save = []
name = []
first = True
sections = []


for line in a:
    if line.startswith("0"):
            save.append(int("0x" + line.split()[0], 16))
            name.append(line.split()[1])

save.append(1000)

for i in range(len(name)):
        sections.append(Section(save[i], save[i+1], name[i]))

first = True
for line in f:
    if first:
        r.write(line + "\n")
        first = False
    else:
        pc = int(line.split()[3], 16)
        for i in range(len(sections)):
            if sections[i].containing(pc):
                r.write(line.rstrip() + "\t" + sections[i].name + "\n")


