add wave -group "Core"                       /tb/top_i/core_region_i/RISCV_CORE/*
add wave -group "IF Stage" -group "Prefetch" /tb/top_i/core_region_i/RISCV_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/*
add wave -group "IF Stage"                   /tb/top_i/core_region_i/RISCV_CORE/if_stage_i/*
add wave -group "ID Stage"                   /tb/top_i/core_region_i/RISCV_CORE/id_stage_i/*
add wave -group "RF"                         /tb/top_i/core_region_i/RISCV_CORE/id_stage_i/registers_i/MemContentxDP
add wave -group "Decoder"                    /tb/top_i/core_region_i/RISCV_CORE/id_stage_i/decoder_i/*
add wave -group "Controller"                 /tb/top_i/core_region_i/RISCV_CORE/id_stage_i/controller_i/*
add wave -group "Exc Ctrl"                   /tb/top_i/core_region_i/RISCV_CORE/id_stage_i/exc_controller_i/*
add wave -group "EX Stage" -group "ALU"      /tb/top_i/core_region_i/RISCV_CORE/ex_stage_i/alu_i/*
add wave -group "EX Stage" -group "MUL"      /tb/top_i/core_region_i/RISCV_CORE/ex_stage_i/mult_i/*
add wave -group "EX Stage"                   /tb/top_i/core_region_i/RISCV_CORE/ex_stage_i/*
add wave -group "LSU"                        /tb/top_i/core_region_i/RISCV_CORE/load_store_unit_i/*
add wave -group "CSR"                        /tb/top_i/core_region_i/RISCV_CORE/cs_registers_i/*
add wave -group "Debug"                      /tb/top_i/core_region_i/RISCV_CORE/debug_unit_i/*

configure wave -namecolwidth  250
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 1
configure wave -timelineunits ns
