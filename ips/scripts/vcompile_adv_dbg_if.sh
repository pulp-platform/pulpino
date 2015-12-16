#!/bin/tcsh
source ${IPS_PATH}/scripts/colors.sh

##############################################################################
# Settings
##############################################################################

set IP=adv_dbg_if
set IP_NAME="Advanced Debug Interface"


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
set IP_PATH="${IPS_PATH}/${IP}/rtl"

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

vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/adbg_axi_biu.sv         || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/adbg_axi_module.sv      || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/adbg_crc32.v            || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/adbg_or1k_biu.sv        || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/adbg_or1k_module.sv     || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/adbg_or1k_status_reg.sv || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/adbg_top.sv             || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/bytefifo.v              || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/syncflop.v              || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/syncreg.v               || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/adbg_tap_top.v          || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/adv_dbg_if.sv           || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH} ${IP_PATH}/adbg_axionly_top.sv     || goto error

echo "${Cyan}--> ${IP_NAME} compilation complete! ${NC}"
exit 0

##############################################################################
# Error handler
##############################################################################

error:
echo "${NC}"
exit 1
