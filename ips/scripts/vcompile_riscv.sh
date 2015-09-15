#!/bin/tcsh
source scripts/colors.sh

set IP=riscv
set IP_NAME="RISCV processor core"

echo "${Green}--> Compiling ${IP_NAME}... ${NC}"

rm -rf ${MSIM_LIBS_PATH}/${IP}_lib

vlib ${MSIM_LIBS_PATH}/${IP}_lib
vmap ${IP}_lib ${MSIM_LIBS_PATH}/${IP}_lib

echo "${Green}Compiling component:   ${Brown} ${IP_NAME} ${NC}"
echo "${Red}"

vlog -quiet -sv -work ${IP}_lib +incdir+${IPS_PATH}/${IP}/include ${IPS_PATH}/${IP}/riscv_core.sv             || goto error
vlog -quiet -sv -work ${IP}_lib +incdir+${IPS_PATH}/${IP}/include ${IPS_PATH}/${IP}/prefetch_buffer.sv        || goto error
vlog -quiet -sv -work ${IP}_lib +incdir+${IPS_PATH}/${IP}/include ${IPS_PATH}/${IP}/if_stage.sv               || goto error
vlog -quiet -sv -work ${IP}_lib +incdir+${IPS_PATH}/${IP}/include ${IPS_PATH}/${IP}/id_stage.sv               || goto error
vlog -quiet -sv -work ${IP}_lib +incdir+${IPS_PATH}/${IP}/include ${IPS_PATH}/${IP}/controller.sv             || goto error
vlog -quiet -sv -work ${IP}_lib +incdir+${IPS_PATH}/${IP}/include ${IPS_PATH}/${IP}/exc_controller.sv         || goto error
vlog -quiet -sv -work ${IP}_lib +incdir+${IPS_PATH}/${IP}/include ${IPS_PATH}/${IP}/hwloop_controller.sv      || goto error
vlog -quiet -sv -work ${IP}_lib +incdir+${IPS_PATH}/${IP}/include ${IPS_PATH}/${IP}/hwloop_regs.sv            || goto error
vlog -quiet -sv -work ${IP}_lib +incdir+${IPS_PATH}/${IP}/include ${IPS_PATH}/${IP}/register_file.sv          || goto error
vlog -quiet -sv -work ${IP}_lib +incdir+${IPS_PATH}/${IP}/include ${IPS_PATH}/${IP}/compressed_decoder.sv     || goto error
vlog -quiet -sv -work ${IP}_lib +incdir+${IPS_PATH}/${IP}/include ${IPS_PATH}/${IP}/ex_stage.sv               || goto error
vlog -quiet -sv -work ${IP}_lib +incdir+${IPS_PATH}/${IP}/include ${IPS_PATH}/${IP}/alu.sv                    || goto error
vlog -quiet -sv -work ${IP}_lib +incdir+${IPS_PATH}/${IP}/include ${IPS_PATH}/${IP}/mult.sv                   || goto error
vlog -quiet -sv -work ${IP}_lib +incdir+${IPS_PATH}/${IP}/include ${IPS_PATH}/${IP}/cs_registers.sv           || goto error
vlog -quiet -sv -work ${IP}_lib +incdir+${IPS_PATH}/${IP}/include ${IPS_PATH}/${IP}/load_store_unit.sv        || goto error
vlog -quiet -sv -work ${IP}_lib +incdir+${IPS_PATH}/${IP}/include ${IPS_PATH}/${IP}/debug_unit.sv             || goto error

echo "${Cyan}--> ${IP_NAME} compilation complete! ${NC}"
exit 0

error:
echo "${NC}"
exit 1
