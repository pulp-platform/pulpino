if (${GCC_ETH})
  add_application(testALU testALU.c LABELS "riscv_tests" LABELS "verilator_tests")
endif()
