#!/bin/tcsh
source ${IPS_PATH}/scripts/colors.sh

##############################################################################
# Settings
##############################################################################

set IP=axi_slice_dc
set IP_NAME="AXI Slice with Dual Clock FIFOs"


##############################################################################
# Check settings
##############################################################################

# check if environment variables are defined
if (! $?MSIM_LIBS_PATH ) then
  echo "${Red} MSIM_LIBS_PATH is not defined ${NC}"
  exit 1
endif

if (! $?IPS_PATH ) then
  echo "${Red} IPS_PATH is not defined ${NC}"
  exit 1
endif


set LIB_NAME="${IP}_lib"
set LIB_PATH="${MSIM_LIBS_PATH}/${LIB_NAME}"
set IP_PATH="${IPS_PATH}/axi/${IP}"

##############################################################################
# Preparing library
##############################################################################

echo "${Green}--> Compiling ${IP_NAME}... ${NC}"

rm -rf $LIB_PATH

vlib $LIB_PATH
vmap $LIB_NAME $LIB_PATH

echo "${Green}Compiling component: ${Brown} ${IP_NAME} ${NC}"
echo "${Red}"

##############################################################################
# Compiling RTL
##############################################################################

vlog -quiet -sv -work ${LIB_PATH} ${IP_PATH}/dc_data_buffer.v            || goto error
vlog -quiet -sv -work ${LIB_PATH} ${IP_PATH}/dc_full_detector.v          || goto error
vlog -quiet -sv -work ${LIB_PATH} ${IP_PATH}/dc_synchronizer.v           || goto error
vlog -quiet -sv -work ${LIB_PATH} ${IP_PATH}/dc_token_ring_fifo_din.v    || goto error
vlog -quiet -sv -work ${LIB_PATH} ${IP_PATH}/dc_token_ring_fifo_dout.v   || goto error
vlog -quiet -sv -work ${LIB_PATH} ${IP_PATH}/dc_token_ring.v             || goto error
vlog -quiet -sv -work ${LIB_PATH} ${IP_PATH}/axi_slice_dc_master.sv      || goto error
vlog -quiet -sv -work ${LIB_PATH} ${IP_PATH}/axi_slice_dc_slave.sv       || goto error

echo "${Cyan}--> ${IP_NAME} compilation complete! ${NC}"
exit 0

##############################################################################
# Error handler
##############################################################################

error:
echo "${NC}"
exit 1
