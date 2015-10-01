
if { ![info exists ::env(XILINX_PART)] } {
  set ::env(XILINX_PART) "xc7z020clg484-1"
}

if { ![info exists ::env(XILINX_BOARD)] } {
  set ::env(XILINX_BOARD) "em.avnet.com:zynq:zed:c"
}

set partNumber $::env(XILINX_PART)
set boardName  $::env(XILINX_BOARD)

create_project xilinx_mem_32768x32_dp . -part $partNumber
set_property board $boardName [current_project]
create_ip -name blk_mem_gen -vendor xilinx.com -library ip -module_name xilinx_mem_32768x32_dp
set_property -dict [list CONFIG.Memory_Type {True_Dual_Port_RAM} CONFIG.Use_Byte_Write_Enable {true} CONFIG.Byte_Size {8} CONFIG.Write_Width_A {32} CONFIG.Write_Depth_A {32768} CONFIG.Register_PortA_Output_of_Memory_Primitives {false} CONFIG.Register_PortB_Output_of_Memory_Primitives {false} CONFIG.Register_PortB_Output_of_Memory_Core {false} CONFIG.Use_RSTA_Pin {true} CONFIG.Use_RSTB_Pin {true}] [get_ips xilinx_mem_32768x32_dp]
generate_target all [get_files ./xilinx_mem_32768x32_dp.srcs/sources_1/ip/xilinx_mem_32768x32_dp/xilinx_mem_32768x32_dp.xci]
create_ip_run [get_files -of_objects [get_fileset sources_1] ./xilinx_mem_32768x32_dp.srcs/sources_1/ip/xilinx_mem_32768x32_dp/xilinx_mem_32768x32_dp.xci]
launch_run -jobs 8 xilinx_mem_32768x32_dp_synth_1
wait_on_run xilinx_mem_32768x32_dp_synth_1
