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
