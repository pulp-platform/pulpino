// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.


module axi_spi_slave_wrap
#(
    parameter AXI_ADDRESS_WIDTH = 32,
    parameter AXI_DATA_WIDTH    = 64,
    parameter AXI_ID_WIDTH      = 16,
    parameter AXI_USER_WIDTH    = 10
  )
(
    input logic              clk_i,
    input logic              rst_ni,

    input logic              test_mode,

    AXI_BUS.Master           axi_master,

    input  logic             spi_clk,
    input  logic             spi_cs,
    output logic [1:0]       spi_mode,
    output logic             spi_sdo0,
    output logic             spi_sdo1,
    output logic             spi_sdo2,
    output logic             spi_sdo3,
    input  logic             spi_sdi0,
    input  logic             spi_sdi1,
    input  logic             spi_sdi2,
    input  logic             spi_sdi3
  );

  axi_spi_slave
  #(
    .AXI_ADDR_WIDTH ( AXI_ADDRESS_WIDTH ),
    .AXI_DATA_WIDTH ( AXI_DATA_WIDTH    ),
    .AXI_ID_WIDTH   ( AXI_ID_WIDTH      ),
    .AXI_USER_WIDTH ( AXI_USER_WIDTH    )
    )
  axi_spi_slave_i
  (
    .axi_aclk             ( clk_i                ),
    .axi_aresetn          ( rst_ni               ),

    .axi_master_aw_valid  ( axi_master.aw_valid  ),
    .axi_master_aw_id     ( axi_master.aw_id     ),
    .axi_master_aw_prot   ( axi_master.aw_prot   ),
    .axi_master_aw_region ( axi_master.aw_region ),
    .axi_master_aw_qos    ( axi_master.aw_qos    ),
    .axi_master_aw_cache  ( axi_master.aw_cache  ),
    .axi_master_aw_lock   ( axi_master.aw_lock   ),
    .axi_master_aw_burst  ( axi_master.aw_burst  ),
    .axi_master_aw_size   ( axi_master.aw_size   ),
    .axi_master_aw_len    ( axi_master.aw_len    ),
    .axi_master_aw_addr   ( axi_master.aw_addr   ),
    .axi_master_aw_user   ( axi_master.aw_user   ),
    .axi_master_aw_ready  ( axi_master.aw_ready  ),

    .axi_master_w_valid   ( axi_master.w_valid   ),
    .axi_master_w_data    ( axi_master.w_data    ),
    .axi_master_w_strb    ( axi_master.w_strb    ),
    .axi_master_w_last    ( axi_master.w_last    ),
    .axi_master_w_user    ( axi_master.w_user    ),
    .axi_master_w_ready   ( axi_master.w_ready   ),

    .axi_master_b_valid   ( axi_master.b_valid   ),
    .axi_master_b_id      ( axi_master.b_id      ),
    .axi_master_b_resp    ( axi_master.b_resp    ),
    .axi_master_b_user    ( axi_master.b_user    ),
    .axi_master_b_ready   ( axi_master.b_ready   ),

    .axi_master_ar_valid  ( axi_master.ar_valid  ),
    .axi_master_ar_id     ( axi_master.ar_id     ),
    .axi_master_ar_prot   ( axi_master.ar_prot   ),
    .axi_master_ar_region ( axi_master.ar_region ),
    .axi_master_ar_qos    ( axi_master.ar_qos    ),
    .axi_master_ar_cache  ( axi_master.ar_cache  ),
    .axi_master_ar_lock   ( axi_master.ar_lock   ),
    .axi_master_ar_burst  ( axi_master.ar_burst  ),
    .axi_master_ar_size   ( axi_master.ar_size   ),
    .axi_master_ar_len    ( axi_master.ar_len    ),
    .axi_master_ar_addr   ( axi_master.ar_addr   ),
    .axi_master_ar_user   ( axi_master.ar_user   ),
    .axi_master_ar_ready  ( axi_master.ar_ready  ),

    .axi_master_r_valid   ( axi_master.r_valid   ),
    .axi_master_r_id      ( axi_master.r_id      ),
    .axi_master_r_data    ( axi_master.r_data    ),
    .axi_master_r_resp    ( axi_master.r_resp    ),
    .axi_master_r_last    ( axi_master.r_last    ),
    .axi_master_r_user    ( axi_master.r_user    ),
    .axi_master_r_ready   ( axi_master.r_ready   ),

    .test_mode            ( test_mode            ),

    .spi_sclk             ( spi_clk              ),
    .spi_cs               ( spi_cs               ),
    .spi_mode             ( spi_mode             ),
    .spi_sdo0             ( spi_sdo0             ),
    .spi_sdo1             ( spi_sdo1             ),
    .spi_sdo2             ( spi_sdo2             ),
    .spi_sdo3             ( spi_sdo3             ),
    .spi_sdi0             ( spi_sdi0             ),
    .spi_sdi1             ( spi_sdi1             ),
    .spi_sdi2             ( spi_sdi2             ),
    .spi_sdi3             ( spi_sdi3             )
  );
endmodule
