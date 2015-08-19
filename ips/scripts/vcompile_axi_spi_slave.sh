#!/bin/tcsh

source scripts/colors.sh

echo "${Green}--> Compiling AXI SPI SLAVE INTERFACE... ${NC}"

echo "${Green}library: axi_spi_slave_lib ${NC}"
rm -rf ${MSIM_LIBS_PATH}/axi_spi_slave_lib

vlib ${MSIM_LIBS_PATH}/axi_spi_slave_lib
vmap axi_spi_slave_lib ${MSIM_LIBS_PATH}/axi_spi_slave_lib

echo "${Green}Compiling component:   ${Brown} axi_spi_slave ${NC}"
echo "${Red}"

vlog -work axi_spi_slave_lib -quiet -sv ${IPS_PATH}/axi/axi_spi_slave/spi_slave_axi_plug.sv   || exit 1
vlog -work axi_spi_slave_lib -quiet -sv ${IPS_PATH}/axi/axi_spi_slave/spi_slave_cmd_parser.sv || exit 1
vlog -work axi_spi_slave_lib -quiet -sv ${IPS_PATH}/axi/axi_spi_slave/spi_slave_dc_fifo.sv    || exit 1
vlog -work axi_spi_slave_lib -quiet -sv ${IPS_PATH}/axi/axi_spi_slave/spi_slave_regs.sv       || exit 1
vlog -work axi_spi_slave_lib -quiet -sv ${IPS_PATH}/axi/axi_spi_slave/spi_slave_controller.sv || exit 1
vlog -work axi_spi_slave_lib -quiet -sv ${IPS_PATH}/axi/axi_spi_slave/spi_slave_rx.sv         || exit 1
vlog -work axi_spi_slave_lib -quiet -sv ${IPS_PATH}/axi/axi_spi_slave/spi_slave_tx.sv         || exit 1
vlog -work axi_spi_slave_lib -quiet -sv ${IPS_PATH}/axi/axi_spi_slave/spi_slave_syncro.sv     || exit 1
vlog -work axi_spi_slave_lib -quiet -sv ${IPS_PATH}/axi/axi_spi_slave/axi_spi_slave.sv        || exit 1

echo "${Cyan}--> AXI SPI SLAVE INTERFACE compilation complete! ${NC}"

