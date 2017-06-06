if (${GCC_MARCH} MATCHES "IMXpulpv2")
  add_application(testBitManipulation testBitManipulation.c LABELS "riscv_tests" "verilator_tests")
endif()
