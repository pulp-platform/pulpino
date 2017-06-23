
if { ![info exists ::env(XILINX_PART)] } {
  set ::env(XILINX_PART) "xc7z020clg484-1"
}

if { ![info exists ::env(XILINX_BOARD)] } {
  set ::env(XILINX_BOARD) "em.avnet.com:zynq:zed:c"
}

set partNumber $::env(XILINX_PART)
set boardName  $::env(XILINX_BOARD)

create_project xilinx_fp_fma . -part $partNumber
set_property board $boardName [current_project]
create_ip -name floating_point -vendor xilinx.com -library ip -version 7.0 -module_name xilinx_fp_fma
set_property -dict [list CONFIG.Operation_Type {FMA} CONFIG.Flow_Control {NonBlocking} CONFIG.Axi_Optimize_Goal {Resources} CONFIG.Has_RESULT_TREADY {false} CONFIG.Maximum_Latency {false} CONFIG.Has_ARESETn {true} CONFIG.C_Has_UNDERFLOW {true} CONFIG.C_Has_OVERFLOW {true} CONFIG.C_Has_INVALID_OP {true} CONFIG.A_Precision_Type {Single} CONFIG.C_A_Exponent_Width {8} CONFIG.C_A_Fraction_Width {24} CONFIG.Result_Precision_Type {Single} CONFIG.C_Result_Exponent_Width {8} CONFIG.C_Result_Fraction_Width {24} CONFIG.C_Mult_Usage {Medium_Usage} CONFIG.C_Latency {2} CONFIG.C_Rate {1}] [get_ips xilinx_fp_fma]
generate_target all [get_files ./xilinx_fp_fma.srcs/sources_1/ip/xilinx_fp_fma/xilinx_fp_fma.xci]
create_ip_run [get_files -of_objects [get_fileset sources_1] ./xilinx_fp_fma.srcs/sources_1/ip/xilinx_fp_fma/xilinx_fp_fma.xci]
launch_run -jobs 8 xilinx_fp_fma_synth_1
wait_on_run xilinx_fp_fma_synth_1
