source tcl_files/config/vsim_ips.tcl

set cmd "vsim -quiet $TB \
  -L pulpino_lib \
  $VSIM_IP_LIBS \
  +nowarnTRAN \
  +nowarnTSCALE \
  +nowarnTFMPC \
  +MEMLOAD=$MEMLOAD \
  -t ps \
  -voptargs=\"+acc -suppress 2103\" \
  $VSIM_FLAGS"

eval $cmd

# check exit status in tb and quit the simulation accordingly
proc run_and_exit {} {
  run -all
  quit -code [examine -radix decimal sim:/tb/exit_status]
}
