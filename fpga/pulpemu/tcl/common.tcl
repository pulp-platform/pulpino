source ../common/common.tcl

if { ![info exists ::env(BOARD) ]} {
  set ::env(BOARD) "zedboard"
}

if { ![info exists ::env(XILINX_PART)] } {
  if {[string equal $::env(BOARD) "zybo"]} {
    puts "Running implementation for ZYBO board"
    set ::env(XILINX_PART) "xc7z010clg400-1"
  } {
    set ::env(XILINX_PART) "xc7z020clg484-1"

    if { ![info exists ::env(XILINX_BOARD)] } {
      set ::env(XILINX_BOARD) "em.avnet.com:zed:0.9"
    }
  }
}

if { ![info exists ::env(USE_ZERO_RISCY)] } {
  set ::env(USE_ZERO_RISCY) 0
}
if { ![info exists ::env(RISCY_RV32F)] } {
  set ::env(RISCY_RV32F) 0
}
if { ![info exists ::env(ZERO_RV32M)] } {
  set ::env(ZERO_RV32M) 0
}
if { ![info exists ::env(ZERO_RV32E)] } {
  set ::env(ZERO_RV32E) 0
}


set RTL ../../rtl
set IPS ../../ips
set FPGA_IPS ../ips
set FPGA_RTL ../rtl
set FPGA_PULPINO ../pulpino
