#!/bin/tcsh
source ${PULP_PATH}/vsim/vcompile/colors.csh

##############################################################################
# Settings
##############################################################################

set IP_NAME="work.tb"


##############################################################################
# Check settings
##############################################################################

# check if environment variables are defined
if (! $?VSIM_PATH ) then
  echo "${Red} VSIM_PATH is not defined ${NC}"
  exit 1
endif

if (! $?TB_PATH ) then
  echo "${Red} TB_PATH is not defined ${NC}"
  exit 1
endif

if (! $?RTL_PATH ) then
  echo "${Red} RTL_PATH is not defined ${NC}"
  exit 1
endif

set LIB_NAME="work"
set LIB_PATH="${VSIM_PATH}/${LIB_NAME}"

##############################################################################
# Preparing library
##############################################################################

echo "${Green}--> Compiling ${IP_NAME}... ${NC}"

echo "${Green}Compiling component: ${Brown} ${IP_NAME} ${NC}"
echo "${Red}"

##############################################################################
# Compiling RTL
##############################################################################

vlog -quiet -sv -work ${LIB_PATH} +incdir+${TB_PATH}                                                ${TB_PATH}/uart.sv             || goto error
vlog -quiet -sv -work ${LIB_PATH} +incdir+${TB_PATH} +incdir+${RTL_PATH}/includes/                  ${TB_PATH}/tb.sv               || goto error

vlog -quiet -sv -work ${LIB_PATH}     +incdir+${TB_PATH} -dpiheader ${TB_PATH}/jtag_dpi/dpiheader.h ${TB_PATH}/jtag_dpi.sv         || goto error
vlog -quiet -64 -work ${LIB_PATH} -ccflags "-I${TB_PATH}/jtag_dpi/"                                 ${TB_PATH}/jtag_dpi/jtag_dpi.c || goto error

vlog -quiet -sv -work ${LIB_PATH} +incdir+${TB_PATH} +incdir+${RTL_PATH}/includes/ -dpiheader ${TB_PATH}/mem_dpi/dpiheader.h    ${TB_PATH}/tb.sv || goto error
vlog -quiet -64 -work ${LIB_PATH} -ccflags "-I${TB_PATH}/mem_dpi/"                                  ${TB_PATH}/mem_dpi/mem_dpi.c                 || goto error

echo "${Cyan}--> ${IP_NAME} compilation complete! ${NC}"
exit 0

##############################################################################
# Error handler
##############################################################################

error:
echo "${NC}"
exit 1
