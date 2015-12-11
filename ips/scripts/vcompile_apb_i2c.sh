#!/bin/tcsh
source scripts/colors.sh

set IP=apb_i2c

echo "${Green}--> Compiling APB I2C... ${NC}"

# check if environment variables are defined
if (! $?MSIM_LIBS_PATH ) then
  echo "${Red} MSIM_LIBS_PATH is not defined ${NC}"
  exit 1
endif

if (! $?IPS_PATH ) then
  echo "${Red} IPS_PATH is not defined ${NC}"
  exit 1
endif


echo "${Green}library: apb_i2c_lib ${NC}"
rm -rf ${MSIM_LIBS_PATH}/${IP}_lib

vlib ${MSIM_LIBS_PATH}/${IP}_lib
vmap ${IP}_lib ${MSIM_LIBS_PATH}/${IP}_lib

echo "${Green}Compiling component:   ${Brown} apb_i2c ${NC}"
echo "${Red}"

vlog -work ${IP}_lib -quiet +incdir+${IPS_PATH}/apb/${IP} -sv ${IPS_PATH}/apb/apb_i2c/apb_i2c.sv || exit 1
vlog -work ${IP}_lib -quiet +incdir+${IPS_PATH}/apb/${IP} -sv ${IPS_PATH}/apb/apb_i2c/i2c_master_bit_ctrl.sv || exit 1
vlog -work ${IP}_lib -quiet +incdir+${IPS_PATH}/apb/${IP} -sv ${IPS_PATH}/apb/apb_i2c/i2c_master_byte_ctrl.sv || exit 1

echo "${Cyan}--> APB I2C compilation complete! ${NC}"

