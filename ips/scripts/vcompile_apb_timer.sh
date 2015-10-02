#!/bin/tcsh
source scripts/colors.sh

set IP=apb_timer

echo "${Green}--> Compiling APB Timer... ${NC}"

# check if environment variables are defined
if (! $?MSIM_LIBS_PATH ) then
  echo "${Red} MSIM_LIBS_PATH is not defined ${NC}"
  exit 1
endif

if (! $?IPS_PATH ) then
  echo "${Red} IPS_PATH is not defined ${NC}"
  exit 1
endif


echo "${Green}library: apb_timer_lib ${NC}"
rm -rf ${MSIM_LIBS_PATH}/${IP}_lib

vlib ${MSIM_LIBS_PATH}/${IP}_lib
vmap ${IP}_lib ${MSIM_LIBS_PATH}/${IP}_lib

echo "${Green}Compiling component:   ${Brown} apb_timer ${NC}"
echo "${Red}"

vlog -work ${IP}_lib -quiet +incdir+${IPS_PATH}/apb_peripherals/${IP}/include -sv ${IPS_PATH}/apb_peripherals/apb_timer/apb_timer.sv || exit 1

echo "${Cyan}--> APB TIMER compilation complete! ${NC}"

