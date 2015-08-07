#!/usr/bin/env python
# -*- coding: utf-8 -*-
# sadri - feb - 19 - 2014 - created!

# This python script, reads the value.txt file and generates equivallent tcl script 
# which can be executed by xmd 

L2_address = 0x4c000000 

# read the file 
h = open ( 'value.txt' , 'r' ) 
fileLines = h.readlines()
h.close()

output = []

putAddress = L2_address
for eachLine in fileLines:
	outStr = 'mwr_phys ' + ('0x%x' % putAddress) + ' ' + ('0x%x' % int(eachLine,16)) + '\n' 
	output.append ( outStr ) 
	putAddress += 4 

h = open ( 'value.tcl' , 'w' ) 
for eachLine in output: 
	h.write ( eachLine ) 
h.close() 

