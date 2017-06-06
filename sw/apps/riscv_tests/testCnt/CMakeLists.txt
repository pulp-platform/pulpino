if (${GCC_MARCH} MATCHES "IMXpulpv2")
  add_application(testCnt testCnt.c LABELS "riscv_tests" "verilator_tests")
endif()
