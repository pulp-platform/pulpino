###############################################################################
#                                                                             #
#            ERC Multitherman Lab @ DEI - University of Bologna               #
#                        Viale Pepoli 3/2 - 40136                             #
#                       Bologna - phone 0512092759                            #
#                                                                             #
#  Engineer:       Francesco Conti - f.conti@unibo.it                         #
#                                                                             #
#  Project:        PULP emulator (PULPemu)                                    #
#  File:           pulpemu_regression.tcl                                     #
#  Description:    XMD script to execute PULPemu regression test              #
#                                                                             #
###############################################################################

proc regression_test { test } {
   set PULPEMU_TESTS "~/PULP/pulp2/sw/apps/pulpemu_tests"
   set PUTS_BOLD    [binary format a4 \x1b\x5b\x31\x6d]
   set PUTS_RESET   [binary format a4 \x1b\x5b\x30\x6d]
   set PUTS_RED     [binary format a5 \x1b\x5b\x33\x31\x6d]
   set PUTS_GREEN   [binary format a5 \x1b\x5b\x33\x32\x6d]
   set PUTS_BLUE    [binary format a5 \x1b\x5b\x33\x34\x6d]

   set PWD [pwd]   
   cd ${PULPEMU_TESTS}/${test}
   set testcaps [string toupper ${test}]
   puts "\nExecuting ${PUTS_BOLD}${PUTS_BLUE}${testcaps}${PUTS_RESET}."
   puts [string repeat "*" [expr 11+[string length ${test}]]]
   puts "Compiling test."
   set status [catch {exec make ${test}.tcl >& compile.log} result]
   if { $status != 0 } {
      puts "Compilation ${PUTS_BOLD}${PUTS_RED}failed${PUTS_RESET}."
   } else {
      source xmd.tcl
      set status [catch {exec diff regression.log regression_success.log} result]
      if { $status == 0 } {
         puts "Test ${PUTS_BOLD}${PUTS_GREEN}SUCCESS${PUTS_RESET}."
      } else {
         puts "Test ${PUTS_BOLD}${PUTS_RED}FAILED${PUTS_RESET}."
      }
   }
   cd $PWD
}

connect arm hw
regression_test matrixMulMMU
regression_test conv16
con

