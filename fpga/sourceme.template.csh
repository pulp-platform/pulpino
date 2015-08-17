#!/bin/tcsh

# either "zedboard" or "zc706"
setenv BOARD "zedboard"

if ( "$BOARD" == "zedboard" ) then
  setenv XILINX_PART "xc7z020clg484-1"
  setenv XILINX_BOARD "em.avnet.com:zynq:zed:c"
else
  setenv XILINX_PART "xc7z045ffg900-2"
  setenv XILINX_BOARD "xilinx.com:zynq:zc706:b"
endif

source /usr/ela/pack-e/vivado-2015.1-kgf/Vivado/2015.1/settings64.csh

