#!/bin/tcsh
source scripts/colors.sh

echo "${Green}--> Compiling APB UART INTERFACE... ${NC}"

# check if environment variables are defined
if (! $?MSIM_LIBS_PATH ) then
  echo "${Red} MSIM_LIBS_PATH is not defined ${NC}"
  exit 1
endif

if (! $?IPS_PATH ) then
  echo "${Red} IPS_PATH is not defined ${NC}"
  exit 1
endif


echo "${Green}library: apb_uart_lib ${NC}"
rm -rf ${MSIM_LIBS_PATH}/apb_uart_lib

vlib ${MSIM_LIBS_PATH}/apb_uart_lib
vmap apb_uart_lib ${MSIM_LIBS_PATH}/apb_uart_lib

echo "${Green}Compiling component:   ${Brown} apb_uart_lib ${NC}"
echo "${Red}"

vcom -work apb_uart_lib -quiet ${IPS_PATH}/apb_peripherals/apb_uart/apb_uart.vhd
vcom -work apb_uart_lib -quiet ${IPS_PATH}/apb_peripherals/apb_uart/slib_clock_div.vhd
vcom -work apb_uart_lib -quiet ${IPS_PATH}/apb_peripherals/apb_uart/slib_counter.vhd
vcom -work apb_uart_lib -quiet ${IPS_PATH}/apb_peripherals/apb_uart/slib_edge_detect.vhd
vcom -work apb_uart_lib -quiet ${IPS_PATH}/apb_peripherals/apb_uart/slib_fifo.vhd
vcom -work apb_uart_lib -quiet ${IPS_PATH}/apb_peripherals/apb_uart/slib_input_filter.vhd
vcom -work apb_uart_lib -quiet ${IPS_PATH}/apb_peripherals/apb_uart/slib_input_sync.vhd
vcom -work apb_uart_lib -quiet ${IPS_PATH}/apb_peripherals/apb_uart/slib_mv_filter.vhd
vcom -work apb_uart_lib -quiet ${IPS_PATH}/apb_peripherals/apb_uart/uart_baudgen.vhd
vcom -work apb_uart_lib -quiet ${IPS_PATH}/apb_peripherals/apb_uart/uart_interrupt.vhd
vcom -work apb_uart_lib -quiet ${IPS_PATH}/apb_peripherals/apb_uart/uart_receiver.vhd
vcom -work apb_uart_lib -quiet ${IPS_PATH}/apb_peripherals/apb_uart/uart_transmitter.vhd

echo "${Cyan}--> APB UART INTERFACE compilation complete! ${NC}"



