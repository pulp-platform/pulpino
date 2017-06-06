if (${GCC_MARCH} MATCHES "IMXpulpv2")
  add_application(testClip testClip.c LABELS "riscv_tests" "verilator_tests")
endif()
