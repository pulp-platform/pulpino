# yes I know this looks ugly, but for some reason vsim deso not recognize the VSIM_FLAGS as correct arguments :-/
# e.g. it did not recognize the -pli ./something.so argument
#

set cmd "vsim -quiet imperio_tb \
  -L imperio_lib_ps \
  -L fll_lib \
  -L imperio_components_lib \
  +nowarnTRAN \
  +nowarnTSCALE \
  +nowarnTFMPC \
  -t ps \
  +MEMLOAD=SPI \
  -voptargs=\"+acc -suppress 2103\""

eval $cmd
