if (${GCC_MARCH} MATCHES "RV32IM" OR ${GCC_MARCH} MATCHES "IMXpulpv2")
  add_application(testDivRem testDivRem.c LABELS "riscv_tests" "verilator_tests")
endif()