if (${GCC_ETH})
  add_application(testDotMul testDotMul.c LABELS "riscv_tests" "verilator_tests")
endif()
