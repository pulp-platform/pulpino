if (${GCC_MARCH} MATCHES "[*pulpv2]")
  add_application(testDotMul testDotMul.c LABELS "riscv_tests" "verilator_tests")
endif()
