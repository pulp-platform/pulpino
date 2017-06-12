if (${GCC_MARCH} MATCHES "[pulp]+")
  add_application(testDotMul testDotMul.c LABELS "riscv_tests" "verilator_tests")
endif()
