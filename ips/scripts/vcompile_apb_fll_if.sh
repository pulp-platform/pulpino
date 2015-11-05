#!/bin/tcsh
source scripts/colors.sh

echo "${Green}--> Compiling APB FLL interface... ${NC}"

# check if environment variables are defined
if (! $?MSIM_LIBS_PATH ) then
  echo "${Red} MSIM_LIBS_PATH is not defined ${NC}"
  exit 1
endif

if (! $?IPS_PATH ) then
  echo "${Red} IPS_PATH is not defined ${NC}"
  exit 1
endif


echo "${Green}library: apb_fll_if_lib ${NC}"
rm -rf ${MSIM_LIBS_PATH}/apb_fll_if_lib

vlib ${MSIM_LIBS_PATH}/apb_fll_if_lib
vmap apb_fll_if_lib ${MSIM_LIBS_PATH}/apb_fll_if_lib

echo "${Green}Compiling component:   ${Brown} apb_fll_if ${NC}"
echo "${Red}"

vlog -work apb_fll_if_lib -quiet -sv ${IPS_PATH}/apb_internals/apb_fll_if/apb_fll_if.sv    || exit 1

echo "${Cyan}--> APB fll interface compilation complete! ${NC}"

