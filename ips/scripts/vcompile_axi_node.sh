#!/bin/tcsh
source ${IPS_PATH}/scripts/colors.sh

##############################################################################
# Settings
##############################################################################

set IP=axi_node
set IP_NAME="AXI Node"


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

vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/axi_address_decoder_AR.sv  || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/axi_address_decoder_AW.sv  || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/axi_address_decoder_BR.sv  || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/axi_address_decoder_BW.sv  || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/axi_address_decoder_DW.sv  || goto error

vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/axi_AR_allocator.sv        || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/axi_AW_allocator.sv        || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/axi_BR_allocator.sv        || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/axi_BW_allocator.sv        || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/axi_DW_allocator.sv        || goto error

vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/axi_ArbitrationTree.sv     || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/axi_FanInPrimitive_Req.sv  || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/axi_multiplexer.sv         || goto error

vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/axi_request_block.sv       || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/axi_response_block.sv      || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/axi_RR_Flag_Req.sv         || goto error

vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/axi_node.sv                || goto error

vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/apb_regs_top.sv            || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/axi_regs_top.sv            || goto error

echo "${Cyan}--> ${IP_NAME} compilation complete! ${NC}"
exit 0

##############################################################################
# Error handler
##############################################################################

error:
echo "${NC}"
exit 1
