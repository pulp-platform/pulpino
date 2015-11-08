#!/bin/tcsh
source scripts/colors.sh

set IP=apb_pulpino

echo "${Green}--> Compiling APB Pulpino... ${NC}"

# check if environment variables are defined
if (! $?MSIM_LIBS_PATH ) then
  echo "${Red} MSIM_LIBS_PATH is not defined ${NC}"
  exit 1
endif

if (! $?IPS_PATH ) then
  echo "${Red} IPS_PATH is not defined ${NC}"
  exit 1
endif


echo "${Green}library: apb_pulpino_lib ${NC}"
rm -rf ${MSIM_LIBS_PATH}/${IP}_lib

vlib ${MSIM_LIBS_PATH}/${IP}_lib
vmap ${IP}_lib ${MSIM_LIBS_PATH}/${IP}_lib

echo "${Green}Compiling component:   ${Brown} apb_pulpino ${NC}"
echo "${Red}"

vlog -work ${IP}_lib -quiet +incdir+${IPS_PATH}/apb_peripherals/${IP}/include -sv ${IPS_PATH}/apb_peripherals/${IP}/apb_pulpino.sv || exit 1


echo "${Cyan}--> APB pulpino compilation complete! ${NC}"

