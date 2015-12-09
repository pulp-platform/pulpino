# yes I know this looks ugly, but for some reason vsim deso not recognize the VSIM_FLAGS as correct arguments :-/
# e.g. it did not recognize the -pli ./something.so argument
#

set cmd "vsim -quiet $TB \
  -L imperio_lib_pl \
  -L fll_lib \
  -sdftyp top_i=${::env(VSIM_DIR)}/../imperio/encounter/out/imperio.sdf.gz +sdf_verbose -sdfnoerror\
  +nowarnTRAN \
  +nowarnTSCALE \
  +nowarnTFMPC \
  -v2k_int_delays +no_glitch_msg \
  -t ps \
  -voptargs=\"+acc -suppress 2103\" \
  +MEMLOAD=$MEMLOAD \
  $VSIM_FLAGS"

eval $cmd

source ./tcl_files/pl_disable_synch_checks.tcl

