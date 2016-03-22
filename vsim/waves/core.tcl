# add cores
set rvcores [find instances -recursive -bydu riscv_core]
set lnu [find instances -recursive -bydu lnu_wrapper]
set fpuprivate [find instances -recursive -bydu fpu_private]
set fpushared [find instances -recursive -bydu fpu_shared]

if {$rvcores ne ""} {
  add wave -group "Core"                                     /imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/*
  add wave -group "IF Stage" -group "Hwlp Ctrl"              /imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/if_stage_i/hwloop_controller_i/*
  add wave -group "IF Stage" -group "Prefetch" -group "FIFO" /imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/*
  add wave -group "IF Stage" -group "Prefetch"               /imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/*
  add wave -group "IF Stage"                                 /imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/if_stage_i/*
  add wave -group "ID Stage"                                 /imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/id_stage_i/*
#  add wave -group "RF"                                       /imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/id_stage_i/registers_i/mem
  add wave -group "Decoder"                                  /imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/id_stage_i/decoder_i/*
  add wave -group "Controller"                               /imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/id_stage_i/controller_i/*
  add wave -group "Exc Ctrl"                                 /imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/id_stage_i/exc_controller_i/*
  add wave -group "Hwloop Regs"                              /imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/id_stage_i/hwloop_regs_i/*
  add wave -group "EX Stage" -group "ALU"                    /imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/ex_stage_i/alu_i/*
  add wave -group "EX Stage" -group "ALU_DIV"                /imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/ex_stage_i/alu_i/div_i/*
  add wave -group "EX Stage" -group "MUL"                    /imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/ex_stage_i/mult_i/*
  add wave -group "EX Stage"                                 /imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/ex_stage_i/*
  add wave -group "LSU"                                      /imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/load_store_unit_i/*
  add wave -group "CSR"                                      /imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/cs_registers_i/*
  add wave -group "Debug"                                    /imperio_tb/top_i/pulpino_i/core_region_i/RISCV_CORE/debug_unit_i/*
}

set or1kcores [find instances -recursive -bydu or10n_core]

if {$or1kcores ne ""} {
  add wave -group "Core"                                     /imperio_tb/top_i/pulpino_i/core_region_i/OR10N_CORE/*
  add wave -group "IF Stage"                                 /imperio_tb/top_i/pulpino_i/core_region_i/OR10N_CORE/if_stage_i/*
  add wave -group "ID Stage"                                 /imperio_tb/top_i/pulpino_i/core_region_i/OR10N_CORE/id_stage_i/*
#  add wave -group "RF"                                       /imperio_tb/top_i/pulpino_i/core_region_i/OR10N_CORE/id_stage_i/registers_i/mem
  add wave -group "Controller"                               /imperio_tb/top_i/pulpino_i/core_region_i/OR10N_CORE/id_stage_i/controller_i/*
  add wave -group "Exc Ctrl"                                 /imperio_tb/top_i/pulpino_i/core_region_i/OR10N_CORE/id_stage_i/exc_controller_i/*
  add wave -group "EX Stage" -group "ALU"                    /imperio_tb/top_i/pulpino_i/core_region_i/OR10N_CORE/ex_stage_i/alu_i/*
  add wave -group "EX Stage" -group "MUL"                    /imperio_tb/top_i/pulpino_i/core_region_i/OR10N_CORE/ex_stage_i/mult_i/*
  add wave -group "EX Stage"                                 /imperio_tb/top_i/pulpino_i/core_region_i/OR10N_CORE/ex_stage_i/*
  add wave -group "LSU"                                      /imperio_tb/top_i/pulpino_i/core_region_i/OR10N_CORE/load_store_unit_i/*
  add wave -group "Debug"                                    /imperio_tb/top_i/pulpino_i/core_region_i/OR10N_CORE/debug_unit_i/*
}

configure wave -namecolwidth  250
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 1
configure wave -timelineunits ns
