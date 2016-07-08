if (${GCC_ETH})
  add_application(testALUExt testALUExt.c LABELS "riscv_tests" LABELS "verilator_tests")
endif()
