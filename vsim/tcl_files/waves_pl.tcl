add wave -group "Padframe" \
sim:/imperio_tb/top_i/pad_clk_i \
sim:/imperio_tb/top_i/pad_rstn_i \
sim:/imperio_tb/top_i/pad_clk_sel_i \
sim:/imperio_tb/top_i/pad_clk_standalone_i \
sim:/imperio_tb/top_i/pad_testmode_i \
sim:/imperio_tb/top_i/pad_fetch_enable_i \
sim:/imperio_tb/top_i/pad_ssclk_i \
sim:/imperio_tb/top_i/pad_scs_io \
sim:/imperio_tb/top_i/pad_sio0_io \
sim:/imperio_tb/top_i/pad_sio1_io \
sim:/imperio_tb/top_i/pad_sio2_io \
sim:/imperio_tb/top_i/pad_sio3_io \
sim:/imperio_tb/top_i/pad_msclk_o \
sim:/imperio_tb/top_i/pad_mcs_io \
sim:/imperio_tb/top_i/pad_mio0_io \
sim:/imperio_tb/top_i/pad_mio1_io \
sim:/imperio_tb/top_i/pad_mio2_io \
sim:/imperio_tb/top_i/pad_mio3_io \
sim:/imperio_tb/top_i/pad_rx_i \
sim:/imperio_tb/top_i/pad_tx_o \
sim:/imperio_tb/top_i/pad_rts_o \
sim:/imperio_tb/top_i/pad_cts_i \
sim:/imperio_tb/top_i/pad_scl_io \
sim:/imperio_tb/top_i/pad_sda_io \
sim:/imperio_tb/top_i/pad_gpio_io \
sim:/imperio_tb/top_i/pad_tck_i \
sim:/imperio_tb/top_i/pad_trstn_i \
sim:/imperio_tb/top_i/pad_tms_i \
sim:/imperio_tb/top_i/pad_tdi_i \
sim:/imperio_tb/top_i/pad_tdo_o

add wave -group "Core" \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/clk \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/rst_n \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/test_en_i \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/boot_addr_i \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/core_id_i \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/cluster_id_i \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/instr_req_o \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/instr_gnt_i \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/instr_rvalid_i \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/instr_addr_o \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/instr_rdata_i \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/data_req_o \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/data_gnt_i \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/data_rvalid_i \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/data_we_o \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/data_be_o \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/data_addr_o \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/data_wdata_o \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/data_rdata_i \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/data_err_i \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/irq_i \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/dbginf_stall_i \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/dbginf_bp_o \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/dbginf_strobe_i \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/dbginf_ack_o \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/dbginf_we_i \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/dbginf_addr_i \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/dbginf_data_i \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/dbginf_data_o \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/fetch_enable_i \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/core_busy_o

add wave -group "Instr MEM Top"  \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/instr_mem/clk \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/instr_mem/rst_n \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/instr_mem/en_i \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/instr_mem/addr_i \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/instr_mem/wdata_i \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/instr_mem/rdata_o \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/instr_mem/we_i \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/instr_mem/be_i \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/instr_mem/bypass_en_i

add wave -group "Instr MEM 0" \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/instr_mem/sp_ram_wrap_i_sp_ram_bank_i_sram_bank_gen_0__bank_mem_bank_i_cut/DO \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/instr_mem/sp_ram_wrap_i_sp_ram_bank_i_sram_bank_gen_0__bank_mem_bank_i_cut/DI \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/instr_mem/sp_ram_wrap_i_sp_ram_bank_i_sram_bank_gen_0__bank_mem_bank_i_cut/A \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/instr_mem/sp_ram_wrap_i_sp_ram_bank_i_sram_bank_gen_0__bank_mem_bank_i_cut/DVSE \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/instr_mem/sp_ram_wrap_i_sp_ram_bank_i_sram_bank_gen_0__bank_mem_bank_i_cut/DVS \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/instr_mem/sp_ram_wrap_i_sp_ram_bank_i_sram_bank_gen_0__bank_mem_bank_i_cut/WEB \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/instr_mem/sp_ram_wrap_i_sp_ram_bank_i_sram_bank_gen_0__bank_mem_bank_i_cut/CK \
sim:/imperio_tb/top_i/pulpino_i/core_region_i/instr_mem/sp_ram_wrap_i_sp_ram_bank_i_sram_bank_gen_0__bank_mem_bank_i_cut/CSB

add wave -group "APB SPI Master OE"  \
sim:/imperio_tb/top_i/oe_msio_int

add wave -group "APB SPI Master"  \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/HCLK \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/HRESETn \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/PADDR \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/PWDATA \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/PWRITE \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/PSEL \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/PENABLE \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/PRDATA \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/PREADY \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/PSLVERR \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/events_o \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/spi_clk \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/spi_csn0 \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/spi_csn1 \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/spi_csn2 \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/spi_csn3 \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/spi_mode \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/spi_sdo0 \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/spi_sdo1 \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/spi_sdo2 \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/spi_sdo3 \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/spi_sdi0 \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/spi_sdi1 \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/spi_sdi2 \
sim:/imperio_tb/top_i/pulpino_i/peripherals_i/apb_spi_master_i/spi_sdi3

configure wave -namecolwidth  250
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 1
configure wave -timelineunits ns
