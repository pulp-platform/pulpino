# add cores
set rvcores [find instances -recursive -bydu riscv_core -nodu]

if {$rvcores ne ""} {
  set rvprefetch [find instances -recursive -bydu riscv_prefetch_L0_buffer -nodu]

  add wave -group "Core"                                     $rvcores/*
  add wave -group "IF Stage" -group "Hwlp Ctrl"              $rvcores/if_stage_i/hwloop_controller_i/*
  if {$rvprefetch ne ""} {
    add wave -group "IF Stage" -group "Prefetch" -group "L0"   $rvcores/if_stage_i/prefetch_128/prefetch_buffer_i/L0_buffer_i/*
    add wave -group "IF Stage" -group "Prefetch"               $rvcores/if_stage_i/prefetch_128/prefetch_buffer_i/*
  } {
    add wave -group "IF Stage" -group "Prefetch" -group "FIFO" $rvcores/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/*
    add wave -group "IF Stage" -group "Prefetch"               $rvcores/if_stage_i/prefetch_32/prefetch_buffer_i/*
  }
  add wave -group "IF Stage"                                 $rvcores/if_stage_i/*
  add wave -group "ID Stage"                                 $rvcores/id_stage_i/*
  add wave -group "RF"                                       $rvcores/id_stage_i/registers_i/mem
  add wave -group "Decoder"                                  $rvcores/id_stage_i/decoder_i/*
  add wave -group "Controller"                               $rvcores/id_stage_i/controller_i/*
  add wave -group "Int Ctrl"                                 $rvcores/id_stage_i/int_controller_i/*
  add wave -group "Hwloop Regs"                              $rvcores/id_stage_i/hwloop_regs_i/*
  add wave -group "EX Stage" -group "ALU"                    $rvcores/ex_stage_i/alu_i/*
  add wave -group "EX Stage" -group "ALU_DIV"                $rvcores/ex_stage_i/alu_i/int_div/div_i/*
  add wave -group "EX Stage" -group "MUL"                    $rvcores/ex_stage_i/mult_i/*
  add wave -group "EX Stage"                                 $rvcores/ex_stage_i/*
  add wave -group "LSU"                                      $rvcores/load_store_unit_i/*
  add wave -group "CSR"                                      $rvcores/cs_registers_i/*
  add wave -group "Debug"                                    $rvcores/debug_unit_i/*
}

set or10n_core [find instances -recursive -bydu or10n_core -nodu]
set lnu [find instances -recursive -bydu lnu_wrapper]
set fpuprivate [find instances -recursive -bydu fpu_private]
set fpushared [find instances -recursive -bydu fpu_shared]

if {$or10n_core ne ""} {
  add wave -group "Core"                                     $or10n_core/*
  add wave -group "IF Stage"                                 $or10n_core/if_stage_i/*
  add wave -group "ID Stage"                                 $or10n_core/id_stage_i/*
  add wave -group "RF"                                       $or10n_core/id_stage_i/registers_i/mem
  add wave -group "Controller"                               $or10n_core/id_stage_i/controller_i/*
  add wave -group "Exc Ctrl"                                 $or10n_core/id_stage_i/exc_controller_i/*
  add wave -group "EX Stage" -group "ALU"                    $or10n_core/ex_stage_i/alu_i/*
  add wave -group "EX Stage" -group "MUL"                    $or10n_core/ex_stage_i/mult_i/*
  add wave -group "EX Stage"                                 $or10n_core/ex_stage_i/*
  add wave -group "LSU"                                      $or10n_core/load_store_unit_i/*
  add wave -group "Debug"                                    $or10n_core/debug_unit_i/*
}

configure wave -namecolwidth  250
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 1
configure wave -timelineunits ns
