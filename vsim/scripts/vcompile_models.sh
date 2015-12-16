#!/bin/tcsh
source ${PULP_PATH}/vsim/scripts/colors.sh

##############################################################################
# Settings
##############################################################################

set IP=models
set IP_NAME="Simulation models"


##############################################################################
# Check settings
##############################################################################

# check if environment variables are defined
if (! $?MSIM_LIBS_PATH ) then
  echo "${Red} MSIM_LIBS_PATH is not defined ${NC}"
  exit 1
endif

if (! $?TB_PATH ) then
  echo "${Red} TB_PATH is not defined ${NC}"
  exit 1
endif

set LIB_NAME="${IP}_lib"
set LIB_PATH="${MSIM_LIBS_PATH}/${LIB_NAME}"

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

vlog -quiet     -work ${LIB_PATH} +define+SPEEDSIM ${IMPERIO_PATH}/models/spi_flash/S25fs256s/model/s25fs256s.v         || goto error
vlog -quiet     -work ${LIB_PATH}                  ${IMPERIO_PATH}/models/24xx1025/24FC1025.v                           || goto error

echo "${Cyan}--> ${IP_NAME} compilation complete! ${NC}"
exit 0

##############################################################################
# Error handler
##############################################################################

error:
echo "${NC}"
exit 1
