if (${GCC_MARCH} MATCHES "RV32IM" OR ${GCC_MARCH} MATCHES "IMXpulpv2")
  add_application(testMUL testMUL.c LABELS "riscv_tests" "verilator_tests")
endif()