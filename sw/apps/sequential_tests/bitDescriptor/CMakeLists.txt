if(!${RISCV})
  set_source_files_properties(
    bitDescriptor.c
    PROPERTIES COMPILE_FLAGS "-mfbit")
endif()

add_application(bitDescriptor bitDescriptor.c LABELS "sequential_tests" "verilator_tests")
