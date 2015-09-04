#!/bin/tcsh
source scripts/colors.sh

# decide if we want to build for riscv or or1k
if ( ! $?PULP_CORE) then
  set PULP_CORE="riscv"
endif

if ( $PULP_CORE == "riscv" ) then
  set CORE_DEFINES=+define+RISCV
  echo "${Yellow} Compiling for RISCV core ${NC}"
else
  set CORE_DEFINES=+define+OR10N
  echo "${Yellow} Compiling for OR10N core ${NC}"
endif



echo "${Green}--> Compiling standalone files... ${NC}"

rm -rf ${MSIM_LIBS_PATH}/standalone_lib

vlib ${MSIM_LIBS_PATH}/standalone_lib
vmap standalone_lib ${MSIM_LIBS_PATH}/standalone_lib

echo "${Green}Compiling component:   ${Brown} standalone ${NC}"
echo "${Red}"

# files depending on RISCV vs. OR1K
vlog -quiet -sv -work standalone_lib +incdir+${RTL_PATH}/include $CORE_DEFINES ${RTL_PATH}/core_region.sv  || exit 1

vlog -quiet -sv -work standalone_lib +incdir+${RTL_PATH}/include ${RTL_PATH}/boot_rom_wrap.sv      || exit 1
vlog -quiet -sv -work standalone_lib +incdir+${RTL_PATH}/include ${RTL_PATH}/instr_ram_wrap.sv     || exit 1
vlog -quiet -sv -work standalone_lib +incdir+${RTL_PATH}/include ${RTL_PATH}/sp_ram.sv             || exit 1
vlog -quiet -sv -work standalone_lib +incdir+${RTL_PATH}/include ${RTL_PATH}/sp_ram_wrap.sv        || exit 1
vlog -quiet -sv -work standalone_lib +incdir+${RTL_PATH}/include ${RTL_PATH}/core2axi.sv           || exit 1
vlog -quiet -sv -work standalone_lib +incdir+${RTL_PATH}/include ${RTL_PATH}/ram_mux.sv            || exit 1
vlog -quiet -sv -work standalone_lib +incdir+${RTL_PATH}/include ${RTL_PATH}/axi_node_intf_wrap.sv || exit 1
vlog -quiet -sv -work standalone_lib +incdir+${RTL_PATH}/include ${RTL_PATH}/top.sv                || exit 1
vlog -quiet -sv -work standalone_lib +incdir+${RTL_PATH}/include ${RTL_PATH}/peripherals.sv        || exit 1
vlog -quiet -sv -work standalone_lib +incdir+${RTL_PATH}/include ${RTL_PATH}/axi2apb_wrap.sv       || exit 1
vlog -quiet -sv -work standalone_lib +incdir+${RTL_PATH}/include ${RTL_PATH}/axi_spi_slave_wrap.sv || exit 1
vlog -quiet -sv -work standalone_lib +incdir+${RTL_PATH}/include ${RTL_PATH}/timer_unit.sv         || exit 1
vlog -quiet -sv -work standalone_lib +incdir+${RTL_PATH}/include ${RTL_PATH}/axi_mem_if_SP_wrap.sv || exit 1

echo "${Cyan}--> standalone compilation complete! ${NC}"
