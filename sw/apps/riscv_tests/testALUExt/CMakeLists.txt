if (${GCC_ETH})
  add_application(testALUExt testALUExt.c LABELS "riscv_tests" "verilator_tests")
endif()
