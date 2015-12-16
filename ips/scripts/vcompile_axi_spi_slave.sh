#!/bin/tcsh
source ${IPS_PATH}/scripts/colors.sh

##############################################################################
# Settings
##############################################################################

set IP=axi_spi_slave
set IP_NAME="AXI SPI Slave"


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

vlog -quiet -sv -work ${LIB_PATH} ${IP_PATH}/spi_slave_axi_plug.sv   || goto error
vlog -quiet -sv -work ${LIB_PATH} ${IP_PATH}/spi_slave_cmd_parser.sv || goto error
vlog -quiet -sv -work ${LIB_PATH} ${IP_PATH}/spi_slave_dc_fifo.sv    || goto error
vlog -quiet -sv -work ${LIB_PATH} ${IP_PATH}/spi_slave_regs.sv       || goto error
vlog -quiet -sv -work ${LIB_PATH} ${IP_PATH}/spi_slave_controller.sv || goto error
vlog -quiet -sv -work ${LIB_PATH} ${IP_PATH}/spi_slave_rx.sv         || goto error
vlog -quiet -sv -work ${LIB_PATH} ${IP_PATH}/spi_slave_tx.sv         || goto error
vlog -quiet -sv -work ${LIB_PATH} ${IP_PATH}/spi_slave_syncro.sv     || goto error
vlog -quiet -sv -work ${LIB_PATH} ${IP_PATH}/axi_spi_slave.sv        || goto error

echo "${Cyan}--> ${IP_NAME} compilation complete! ${NC}"
exit 0

##############################################################################
# Error handler
##############################################################################

error:
echo "${NC}"
exit 1
