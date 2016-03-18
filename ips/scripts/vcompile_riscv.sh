#!/bin/tcsh
source ${IPS_PATH}/scripts/colors.sh

##############################################################################
# Settings
##############################################################################

set IP=riscv
set IP_NAME="RISCV processor core"


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
set IP_PATH="${IPS_PATH}/${IP}"

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

vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH}/include ${IP_PATH}/riscv_core.sv             || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH}/include ${IP_PATH}/prefetch_buffer.sv        || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH}/include ${IP_PATH}/prefetch_L0_buffer.sv     || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH}/include ${IP_PATH}/if_stage.sv               || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH}/include ${IP_PATH}/id_stage.sv               || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH}/include ${IP_PATH}/exc_controller.sv         || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH}/include ${IP_PATH}/hwloop_controller.sv      || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH}/include ${IP_PATH}/hwloop_regs.sv            || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH}/include ${IP_PATH}/register_file.sv          || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH}/include ${IP_PATH}/compressed_decoder.sv     || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH}/include ${IP_PATH}/controller.sv             || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH}/include ${IP_PATH}/decoder.sv                || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH}/include ${IP_PATH}/ex_stage.sv               || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH}/include ${IP_PATH}/alu.sv                    || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH}/include ${IP_PATH}/alu_div.sv                || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH}/include ${IP_PATH}/mult.sv                   || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH}/include ${IP_PATH}/cs_registers.sv           || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH}/include ${IP_PATH}/load_store_unit.sv        || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH}/include ${IP_PATH}/debug_unit.sv             || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${IP_PATH}/include ${IP_PATH}/riscv_tracer.sv           || goto error

echo "${Cyan}--> ${IP_NAME} compilation complete! ${NC}"
exit 0

##############################################################################
# Error handler
##############################################################################

error:
echo "${NC}"
exit 1
