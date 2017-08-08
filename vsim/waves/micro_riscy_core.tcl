# add cores
set rvcores [find instances -recursive -bydu zeroriscy_core -nodu]

if {$rvcores ne ""} {
  set rvprefetch [find instances -recursive -bydu riscv_prefetch_L0_buffer -nodu]

  add wave -group "Core"                                     $rvcores/*
  add wave -group "IF Stage" -group "Prefetch" -group "FIFO" $rvcores/if_stage_i/prefetch_buffer_i/fifo_i/*
  add wave -group "IF Stage" -group "Prefetch"               $rvcores/if_stage_i/prefetch_buffer_i/*
  add wave -group "IF Stage"                                 $rvcores/if_stage_i/*
  add wave -group "ID Stage"                                 $rvcores/id_stage_i/*
  add wave -group "RF"                                       $rvcores/id_stage_i/registers_i/mem
  add wave -group "Decoder"                                  $rvcores/id_stage_i/decoder_i/*
  add wave -group "Controller"                               $rvcores/id_stage_i/controller_i/*
  add wave -group "Int Ctrl"                                 $rvcores/id_stage_i/int_controller_i/*
  add wave -group "EX Block" -group "ALU"                    $rvcores/ex_block_i/alu_i/*
  add wave -group "EX Block"                                 $rvcores/ex_block_i/*
  add wave -group "LSU"                                      $rvcores/load_store_unit_i/*
  add wave -group "CSR"                                      $rvcores/cs_registers_i/*
  add wave -group "Debug"                                    $rvcores/debug_unit_i/*
}

configure wave -namecolwidth  250
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 1
configure wave -timelineunits ns
