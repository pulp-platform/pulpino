if (${GCC_MARCH} MATCHES "IMXpulpv2")
  add_application(testVecCmp testVecCmp.c LABELS "riscv_tests" "verilator_tests")
endif()