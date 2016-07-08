if (${GCC_ETH})
  add_application(testBitManipulation testBitManipulation.c LABELS "riscv_tests" LABELS "verilator_tests")
endif()
