if (${GCC_MARCH} MATCHES "IMXpulpv2")
  add_application(testALUExt testALUExt.c LABELS "riscv_tests" "verilator_tests")
endif()
