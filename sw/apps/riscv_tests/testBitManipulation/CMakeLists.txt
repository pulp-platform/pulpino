if (${GCC_ETH})
  add_application(testBitManipulation testBitManipulation.c LABELS "riscv_tests" "verilator_tests")
endif()
