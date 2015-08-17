
################################################################
# This is a generated script based on design: ps7
#
# Though there are limitations about the generated script,
# the main purpose of this utility is to make learning
# IP Integrator Tcl commands easier.
################################################################

################################################################
# Check if script is running in correct Vivado version.
################################################################
set scripts_vivado_version 2015.1
set current_vivado_version [version -short]

if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
   puts ""
   puts "ERROR: This script was generated using Vivado <$scripts_vivado_version> and is being run in <$current_vivado_version> of Vivado. Please run the script in Vivado <$scripts_vivado_version> then open the design in Vivado <$current_vivado_version>. Upgrade the design by running \"Tools => Report => Report IP Status...\", then run write_bd_tcl to create an updated script."

   return 1
}

################################################################
# START
################################################################

# To test this script, run the following commands from Vivado Tcl console:
# source ps7_script.tcl

# If you do not already have a project created,
# you can create a project using the following command:
#    create_project project_1 myproj -part xc7z045ffg900-2
#    set_property BOARD_PART xilinx.com:zc706:part0:0.9 [current_project]


# CHANGE DESIGN NAME HERE
set design_name ps7

# If you do not already have an existing IP Integrator design open,
# you can create a design using the following command:
#    create_bd_design $design_name

# CHECKING IF PROJECT EXISTS
if { [get_projects -quiet] eq "" } {
   puts "ERROR: Please open or create a project!"
   return 1
}


# Creating design if needed
set errMsg ""
set nRet 0

set cur_design [current_bd_design -quiet]
set list_cells [get_bd_cells -quiet]

if { ${design_name} eq "" } {
   # USE CASES:
   #    1) Design_name not set

   set errMsg "ERROR: Please set the variable <design_name> to a non-empty value."
   set nRet 1

} elseif { ${cur_design} ne "" && ${list_cells} eq "" } {
   # USE CASES:
   #    2): Current design opened AND is empty AND names same.
   #    3): Current design opened AND is empty AND names diff; design_name NOT in project.
   #    4): Current design opened AND is empty AND names diff; design_name exists in project.

   if { $cur_design ne $design_name } {
      puts "INFO: Changing value of <design_name> from <$design_name> to <$cur_design> since current design is empty."
      set design_name [get_property NAME $cur_design]
   }
   puts "INFO: Constructing design in IPI design <$cur_design>..."

} elseif { ${cur_design} ne "" && $list_cells ne "" && $cur_design eq $design_name } {
   # USE CASES:
   #    5) Current design opened AND has components AND same names.

   set errMsg "ERROR: Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 1
} elseif { [get_files -quiet ${design_name}.bd] ne "" } {
   # USE CASES: 
   #    6) Current opened design, has components, but diff names, design_name exists in project.
   #    7) No opened design, design_name exists in project.

   set errMsg "ERROR: Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 2

} else {
   # USE CASES:
   #    8) No opened design, design_name not in project.
   #    9) Current opened design, has components, but diff names, design_name not in project.

   puts "INFO: Currently there is no design <$design_name> in project, so creating one..."

   create_bd_design $design_name

   puts "INFO: Making design <$design_name> as current_bd_design."
   current_bd_design $design_name

}

puts "INFO: Currently the variable <design_name> is equal to \"$design_name\"."

if { $nRet != 0 } {
   puts $errMsg
   return $nRet
}

##################################################################
# DESIGN PROCs
##################################################################



# Procedure to create entire design; Provide argument to make
# procedure reusable. If parentCell is "", will use root.
proc create_root_design { parentCell } {

  if { $parentCell eq "" } {
     set parentCell [get_bd_cells /]
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     puts "ERROR: Unable to find parent cell <$parentCell>!"
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     puts "ERROR: Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj


  # Create interface ports
  set DDR [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:ddrx_rtl:1.0 DDR ]
  set FIXED_IO [ create_bd_intf_port -mode Master -vlnv xilinx.com:display_processing_system7:fixedio_rtl:1.0 FIXED_IO ]
  set clking_axi [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 clking_axi ]
  set_property -dict [ list CONFIG.ADDR_WIDTH {32} CONFIG.DATA_WIDTH {32} CONFIG.FREQ_HZ {50000000} CONFIG.PROTOCOL {AXI4LITE}  ] $clking_axi
  set rab_lite [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 rab_lite ]
  set_property -dict [ list CONFIG.ADDR_WIDTH {32} CONFIG.DATA_WIDTH {32} CONFIG.FREQ_HZ {100000000} CONFIG.PROTOCOL {AXI4LITE}  ] $rab_lite
  set rab_master [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 rab_master ]
  set_property -dict [ list CONFIG.ADDR_WIDTH {32} CONFIG.ARUSER_WIDTH {6} CONFIG.AWUSER_WIDTH {6} CONFIG.BUSER_WIDTH {6} CONFIG.CLK_DOMAIN {} CONFIG.DATA_WIDTH {64} CONFIG.FREQ_HZ {100000000} CONFIG.ID_WIDTH {6} CONFIG.MAX_BURST_LENGTH {256} CONFIG.NUM_READ_OUTSTANDING {1} CONFIG.NUM_WRITE_OUTSTANDING {1} CONFIG.PHASE {0.000} CONFIG.PROTOCOL {AXI4} CONFIG.READ_WRITE_MODE {READ_WRITE} CONFIG.RUSER_WIDTH {6} CONFIG.SUPPORTS_NARROW_BURST {1} CONFIG.WUSER_WIDTH {6}  ] $rab_master
  set rab_slave [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:aximm_rtl:1.0 rab_slave ]
  set_property -dict [ list CONFIG.ADDR_WIDTH {32} CONFIG.DATA_WIDTH {64} CONFIG.PROTOCOL {AXI4}  ] $rab_slave
  set stdout_slave [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 stdout_slave ]
  set_property -dict [ list CONFIG.ADDR_WIDTH {32} CONFIG.ARUSER_WIDTH {0} CONFIG.AWUSER_WIDTH {0} CONFIG.BUSER_WIDTH {0} CONFIG.CLK_DOMAIN {} CONFIG.DATA_WIDTH {32} CONFIG.FREQ_HZ {100000000} CONFIG.ID_WIDTH {0} CONFIG.MAX_BURST_LENGTH {256} CONFIG.NUM_READ_OUTSTANDING {1} CONFIG.NUM_WRITE_OUTSTANDING {1} CONFIG.PHASE {0.000} CONFIG.PROTOCOL {AXI4} CONFIG.READ_WRITE_MODE {READ_WRITE} CONFIG.RUSER_WIDTH {0} CONFIG.SUPPORTS_NARROW_BURST {1} CONFIG.WUSER_WIDTH {0}  ] $stdout_slave

  # Create ports
  set end_of_operation [ create_bd_port -dir I -from 31 -to 0 end_of_operation ]
  set fetch_enable [ create_bd_port -dir O -from 31 -to 0 fetch_enable ]
  set ps7_clk [ create_bd_port -dir O ps7_clk ]
  set ps7_rst_n [ create_bd_port -dir O ps7_rst_n ]
  set ulpsoc_clk [ create_bd_port -dir I ulpsoc_clk ]

  # Create instance: axi_clock_converter_0, and set properties
  set axi_clock_converter_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_clock_converter:2.1 axi_clock_converter_0 ]
  set_property -dict [ list CONFIG.ID_WIDTH {6}  ] $axi_clock_converter_0

  # Create instance: axi_clock_converter_1, and set properties
  set axi_clock_converter_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_clock_converter:2.1 axi_clock_converter_1 ]

  # Create instance: axi_clock_converter_2, and set properties
  set axi_clock_converter_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_clock_converter:2.1 axi_clock_converter_2 ]

  # Create instance: axi_crossbar_0, and set properties
  set axi_crossbar_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_crossbar:2.1 axi_crossbar_0 ]
  set_property -dict [ list CONFIG.DATA_WIDTH {32} CONFIG.NUM_MI {5} CONFIG.S01_BASE_ID {0x00001000} CONFIG.S02_BASE_ID {0x00002000} CONFIG.S03_BASE_ID {0x00003000} CONFIG.S04_BASE_ID {0x00004000} CONFIG.S05_BASE_ID {0x00005000} CONFIG.S06_BASE_ID {0x00006000} CONFIG.S07_BASE_ID {0x00007000} CONFIG.S08_BASE_ID {0x00008000} CONFIG.S09_BASE_ID {0x00009000} CONFIG.S10_BASE_ID {0x0000a000} CONFIG.S11_BASE_ID {0x0000b000} CONFIG.S12_BASE_ID {0x0000c000} CONFIG.S13_BASE_ID {0x0000d000} CONFIG.S14_BASE_ID {0x0000e000} CONFIG.S15_BASE_ID {0x0000f000}  ] $axi_crossbar_0

  # Create instance: axi_dwidth_converter_0, and set properties
  set axi_dwidth_converter_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_dwidth_converter:2.1 axi_dwidth_converter_0 ]

  # Create instance: axi_protocol_converter_0, and set properties
  set axi_protocol_converter_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_protocol_converter:2.1 axi_protocol_converter_0 ]

  # Create instance: axi_protocol_converter_1, and set properties
  set axi_protocol_converter_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_protocol_converter:2.1 axi_protocol_converter_1 ]
  set_property -dict [ list CONFIG.MI_PROTOCOL {AXI4} CONFIG.SI_PROTOCOL {AXI3}  ] $axi_protocol_converter_1

  # Create instance: axi_protocol_converter_2, and set properties
  set axi_protocol_converter_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_protocol_converter:2.1 axi_protocol_converter_2 ]

  # Create instance: axi_protocol_converter_3, and set properties
  set axi_protocol_converter_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_protocol_converter:2.1 axi_protocol_converter_3 ]

  # Create instance: axi_protocol_converter_4, and set properties
  set axi_protocol_converter_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_protocol_converter:2.1 axi_protocol_converter_4 ]

  # Create instance: axi_pulp_control, and set properties
  set axi_pulp_control [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 axi_pulp_control ]
  set_property -dict [ list CONFIG.C_ALL_INPUTS {1} CONFIG.C_ALL_OUTPUTS_2 {1} CONFIG.C_IS_DUAL {1}  ] $axi_pulp_control

  # Create instance: axi_stdout_controller_ps, and set properties
  set axi_stdout_controller_ps [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_stdout_controller_ps ]
  set_property -dict [ list CONFIG.SINGLE_PORT_BRAM {1}  ] $axi_stdout_controller_ps

  # Create instance: axi_stdout_controller_pulp, and set properties
  set axi_stdout_controller_pulp [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.0 axi_stdout_controller_pulp ]
  set_property -dict [ list CONFIG.DATA_WIDTH {32} CONFIG.SINGLE_PORT_BRAM {1}  ] $axi_stdout_controller_pulp

  # Create instance: processing_system7_0, and set properties
  set processing_system7_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:processing_system7:5.5 processing_system7_0 ]
  set_property -dict [ list CONFIG.PCW_FPGA0_PERIPHERAL_FREQMHZ {50} CONFIG.PCW_S_AXI_HP0_DATA_WIDTH {64} CONFIG.PCW_USE_S_AXI_HP0 {1} CONFIG.preset {ZC706*}  ] $processing_system7_0

  # Create instance: stdout_bram, and set properties
  set stdout_bram [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.2 stdout_bram ]
  set_property -dict [ list CONFIG.Memory_Type {True_Dual_Port_RAM}  ] $stdout_bram

  # Create interface connections
  connect_bd_intf_net -intf_net axi_clock_converter_0_M_AXI [get_bd_intf_ports rab_slave] [get_bd_intf_pins axi_clock_converter_0/M_AXI]
  connect_bd_intf_net -intf_net axi_clock_converter_1_M_AXI [get_bd_intf_pins axi_clock_converter_1/M_AXI] [get_bd_intf_pins axi_protocol_converter_4/S_AXI]
  connect_bd_intf_net -intf_net axi_clock_converter_2_M_AXI [get_bd_intf_pins axi_clock_converter_2/M_AXI] [get_bd_intf_pins axi_protocol_converter_3/S_AXI]
  connect_bd_intf_net -intf_net axi_crossbar_0_M00_AXI [get_bd_intf_pins axi_crossbar_0/M00_AXI] [get_bd_intf_pins axi_protocol_converter_0/S_AXI]
  connect_bd_intf_net -intf_net axi_crossbar_0_M01_AXI [get_bd_intf_pins axi_crossbar_0/M01_AXI] [get_bd_intf_pins axi_dwidth_converter_0/S_AXI]
  connect_bd_intf_net -intf_net axi_crossbar_0_M02_AXI [get_bd_intf_pins axi_crossbar_0/M02_AXI] [get_bd_intf_pins axi_protocol_converter_2/S_AXI]
  connect_bd_intf_net -intf_net axi_crossbar_0_M03_AXI [get_bd_intf_pins axi_crossbar_0/M03_AXI] [get_bd_intf_pins axi_stdout_controller_ps/S_AXI]
  connect_bd_intf_net -intf_net axi_crossbar_0_M04_AXI [get_bd_intf_pins axi_clock_converter_2/S_AXI] [get_bd_intf_pins axi_crossbar_0/M04_AXI]
  connect_bd_intf_net -intf_net axi_dwidth_converter_0_M_AXI [get_bd_intf_pins axi_clock_converter_0/S_AXI] [get_bd_intf_pins axi_dwidth_converter_0/M_AXI]
  connect_bd_intf_net -intf_net axi_protocol_converter_0_M_AXI [get_bd_intf_pins axi_protocol_converter_0/M_AXI] [get_bd_intf_pins axi_pulp_control/S_AXI]
  connect_bd_intf_net -intf_net axi_protocol_converter_1_M_AXI [get_bd_intf_pins axi_crossbar_0/S00_AXI] [get_bd_intf_pins axi_protocol_converter_1/M_AXI]
  connect_bd_intf_net -intf_net axi_protocol_converter_2_M_AXI [get_bd_intf_ports clking_axi] [get_bd_intf_pins axi_protocol_converter_2/M_AXI]
  connect_bd_intf_net -intf_net axi_protocol_converter_3_M_AXI [get_bd_intf_ports rab_lite] [get_bd_intf_pins axi_protocol_converter_3/M_AXI]
  connect_bd_intf_net -intf_net axi_protocol_converter_4_M_AXI [get_bd_intf_pins axi_protocol_converter_4/M_AXI] [get_bd_intf_pins processing_system7_0/S_AXI_HP0]
  connect_bd_intf_net -intf_net axi_stdout_controller_ps_BRAM_PORTA [get_bd_intf_pins axi_stdout_controller_ps/BRAM_PORTA] [get_bd_intf_pins stdout_bram/BRAM_PORTA]
  connect_bd_intf_net -intf_net axi_stdout_controller_pulp_BRAM_PORTA [get_bd_intf_pins axi_stdout_controller_pulp/BRAM_PORTA] [get_bd_intf_pins stdout_bram/BRAM_PORTB]
  connect_bd_intf_net -intf_net processing_system7_0_DDR [get_bd_intf_ports DDR] [get_bd_intf_pins processing_system7_0/DDR]
  connect_bd_intf_net -intf_net processing_system7_0_FIXED_IO [get_bd_intf_ports FIXED_IO] [get_bd_intf_pins processing_system7_0/FIXED_IO]
  connect_bd_intf_net -intf_net processing_system7_0_M_AXI_GP0 [get_bd_intf_pins axi_protocol_converter_1/S_AXI] [get_bd_intf_pins processing_system7_0/M_AXI_GP0]
  connect_bd_intf_net -intf_net rab_master_1 [get_bd_intf_ports rab_master] [get_bd_intf_pins axi_clock_converter_1/S_AXI]
  connect_bd_intf_net -intf_net stdout_slave_1 [get_bd_intf_ports stdout_slave] [get_bd_intf_pins axi_stdout_controller_pulp/S_AXI]

  # Create port connections
  connect_bd_net -net axi_pulp_control_gpio2_io_o [get_bd_ports fetch_enable] [get_bd_pins axi_pulp_control/gpio2_io_o]
  connect_bd_net -net end_of_operation_1 [get_bd_ports end_of_operation] [get_bd_pins axi_pulp_control/gpio_io_i]
  connect_bd_net -net processing_system7_0_FCLK_CLK0 [get_bd_ports ps7_clk] [get_bd_pins axi_clock_converter_0/s_axi_aclk] [get_bd_pins axi_clock_converter_1/m_axi_aclk] [get_bd_pins axi_clock_converter_2/s_axi_aclk] [get_bd_pins axi_crossbar_0/aclk] [get_bd_pins axi_dwidth_converter_0/s_axi_aclk] [get_bd_pins axi_protocol_converter_0/aclk] [get_bd_pins axi_protocol_converter_1/aclk] [get_bd_pins axi_protocol_converter_2/aclk] [get_bd_pins axi_protocol_converter_4/aclk] [get_bd_pins axi_pulp_control/s_axi_aclk] [get_bd_pins axi_stdout_controller_ps/s_axi_aclk] [get_bd_pins processing_system7_0/FCLK_CLK0] [get_bd_pins processing_system7_0/M_AXI_GP0_ACLK] [get_bd_pins processing_system7_0/S_AXI_HP0_ACLK]
  connect_bd_net -net processing_system7_0_FCLK_RESET0_N [get_bd_ports ps7_rst_n] [get_bd_pins axi_clock_converter_0/m_axi_aresetn] [get_bd_pins axi_clock_converter_0/s_axi_aresetn] [get_bd_pins axi_clock_converter_1/m_axi_aresetn] [get_bd_pins axi_clock_converter_1/s_axi_aresetn] [get_bd_pins axi_clock_converter_2/m_axi_aresetn] [get_bd_pins axi_clock_converter_2/s_axi_aresetn] [get_bd_pins axi_crossbar_0/aresetn] [get_bd_pins axi_dwidth_converter_0/s_axi_aresetn] [get_bd_pins axi_protocol_converter_0/aresetn] [get_bd_pins axi_protocol_converter_1/aresetn] [get_bd_pins axi_protocol_converter_2/aresetn] [get_bd_pins axi_protocol_converter_3/aresetn] [get_bd_pins axi_protocol_converter_4/aresetn] [get_bd_pins axi_pulp_control/s_axi_aresetn] [get_bd_pins axi_stdout_controller_ps/s_axi_aresetn] [get_bd_pins axi_stdout_controller_pulp/s_axi_aresetn] [get_bd_pins processing_system7_0/FCLK_RESET0_N]
  connect_bd_net -net ulpsoc_clk_1 [get_bd_ports ulpsoc_clk] [get_bd_pins axi_clock_converter_0/m_axi_aclk] [get_bd_pins axi_clock_converter_1/s_axi_aclk] [get_bd_pins axi_clock_converter_2/m_axi_aclk] [get_bd_pins axi_protocol_converter_3/aclk] [get_bd_pins axi_stdout_controller_pulp/s_axi_aclk]

  # Create address segments
  create_bd_addr_seg -range 0x10000 -offset 0x51000000 [get_bd_addr_spaces processing_system7_0/Data] [get_bd_addr_segs axi_pulp_control/S_AXI/Reg] SEG_axi_pulp_control_Reg
  create_bd_addr_seg -range 0x10000 -offset 0x51020000 [get_bd_addr_spaces processing_system7_0/Data] [get_bd_addr_segs axi_stdout_controller_ps/S_AXI/Mem0] SEG_axi_stdout_controller_ps_Mem0
  create_bd_addr_seg -range 0x10000 -offset 0x51010000 [get_bd_addr_spaces processing_system7_0/Data] [get_bd_addr_segs clking_axi/Reg] SEG_clking_axi_Reg
  create_bd_addr_seg -range 0x10000 -offset 0x51030000 [get_bd_addr_spaces processing_system7_0/Data] [get_bd_addr_segs rab_lite/Reg] SEG_rab_lite_Reg
  create_bd_addr_seg -range 0x10000000 -offset 0x40000000 [get_bd_addr_spaces processing_system7_0/Data] [get_bd_addr_segs rab_slave/Reg] SEG_rab_slave_Reg
  create_bd_addr_seg -range 0x10000 -offset 0xC0000000 [get_bd_addr_spaces stdout_slave] [get_bd_addr_segs axi_stdout_controller_pulp/S_AXI/Mem0] SEG_axi_stdout_controller_pulp_Mem0
  create_bd_addr_seg -range 0x40000000 -offset 0x0 [get_bd_addr_spaces rab_master] [get_bd_addr_segs processing_system7_0/S_AXI_HP0/HP0_DDR_LOWOCM] SEG_processing_system7_0_HP0_DDR_LOWOCM
  

  # Restore current instance
  current_bd_instance $oldCurInst

  save_bd_design
}
# End of create_root_design()


##################################################################
# MAIN FLOW
##################################################################

create_root_design ""


