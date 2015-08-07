#!/usr/bin/env python
# -*- coding: utf-8 -*-
# sadri - dec - 7 - 2013 - Created! 

# This simple python script generates a single memory content file for the L2 memory 

slmFilesLow = ['slm_files/l2_ram_cut0_lo.slm','slm_files/l2_ram_cut1_lo.slm']
slmFilesHigh = ['slm_files/l2_ram_cut0_hi.slm','slm_files/l2_ram_cut1_hi.slm']

memBaseAddress = 0x10000000 

# sub routine : opens the file, reads its lines, extracts values, outputs an array of values 
def readSLMFile ( fileName ) : 
	outputArray = []

	h = open ( fileName, 'r' ) 
	fileLines = h.readlines() 
	h.close() 
	
	prevAddressValue = 0 

	for eachLine in fileLines:
		tmpArray1 = eachLine.split(' ') 
		currentAddressValue = int ( tmpArray1[0][1:] , 16) 

		if currentAddressValue != (prevAddressValue + 1) and ( currentAddressValue > 0 ): 
			print 'Jump in address : currentAddressValue : ' , ('%x' % currentAddressValue) , ' prevAddressValue : ' , ('%x' % prevAddressValue)
		
			# fill in the address jump with zeros
			for addressCounterFiller in range(prevAddressValue+1,currentAddressValue):
				print ' Putting zero for address ' , '%x' % addressCounterFiller
				outputArray.append ( '00000000' ) 
			
		outputArray.append ( tmpArray1[1][:-1] )	# forget about the ending \n 
		
		prevAddressValue = currentAddressValue
	
	return outputArray 
	
# read files. 
slmFilesValuesLow = []
slmFilesValuesHigh = []

for eachFile in slmFilesLow:
	slmFilesValuesLow.append ( readSLMFile ( eachFile ) ) 
for eachFile in slmFilesHigh:
	slmFilesValuesHigh.append ( readSLMFile ( eachFile ) ) 

# make the final array 
finalArray = []
print ' size of slmFilesValuesHigh[0] : ' , len ( slmFilesValuesHigh[0] ) 
print ' size of slmFilesValuesLow[0] : ' , len ( slmFilesValuesLow[0] ) 

for i in range(0,len(slmFilesValuesHigh[0]) ):		# low cut
	finalArray.append ( slmFilesValuesLow[0][i] )
	finalArray.append ( slmFilesValuesHigh[0][i] ) 
for i in range(0,len(slmFilesValuesHigh[1]) ):		# high cut 
	finalArray.append ( slmFilesValuesLow[1][i] )
	finalArray.append ( slmFilesValuesHigh[1][i] ) 
	
# write out output files 	
# first a simple file which contains the address and the value 
h = open ('address_value.txt', 'w' )
addressCounter = 0
for eachItem in finalArray:
	h.write ( '%08x' % (memBaseAddress+addressCounter) + ' ' + eachItem + '\n' ) 
	addressCounter += 4
h.close()

# second, the file which will be read by the driver subsystem , just the values themselves
h = open ( 'value.txt' , 'w' ) 
for eachItem in finalArray:
	h.write ( eachItem + '\n' ) 
h.close()

print 'done!' 
