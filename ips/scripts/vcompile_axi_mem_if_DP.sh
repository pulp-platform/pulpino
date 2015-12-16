#!/bin/tcsh
source ${IPS_PATH}/scripts/colors.sh

##############################################################################
# Settings
##############################################################################

set IP=axi_mem_if_DP
set IP_NAME="AXI Memory interface Dual-Port"


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

vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/axi_mem_if_SP.sv              || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/axi_mem_if_DP.sv              || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/axi_read_only_ctrl.sv         || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/axi_write_only_ctrl.sv        || goto error

echo "${Cyan}--> ${IP_NAME} compilation complete! ${NC}"
exit 0

##############################################################################
# Error handler
##############################################################################

error:
echo "${NC}"
exit 1
