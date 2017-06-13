source tcl_files/config/vsim_ips.tcl

set cmd "vsim -quiet $TB \
  -L pulpino_lib \
  $VSIM_IP_LIBS \
  +nowarnTRAN \
  +nowarnTSCALE \
  +nowarnTFMPC \
  +MEMLOAD=$MEMLOAD \
  -gUSE_ZERO_RISCY=$env(USE_ZERO_RISCY) \
  -gRISCY_RV32F=$env(RISCY_RV32F) \
  -gZERO_RV32M=$env(ZERO_RV32M) \
  -gZERO_RV32E=$env(ZERO_RV32E) \
  -t ps \
  -voptargs=\"+acc -suppress 2103\" \
  $VSIM_FLAGS"

# set cmd "$cmd -sv_lib ./work/libri5cyv2sim"
eval $cmd

# check exit status in tb and quit the simulation accordingly
proc run_and_exit {} {
  run -all
  quit -code [examine -radix decimal sim:/tb/exit_status]
}
