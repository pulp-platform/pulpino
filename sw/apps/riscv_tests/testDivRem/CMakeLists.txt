if (${GCC_MARCH} MATCHES "RV32IM" OR ${GCC_MARCH} MATCHES "[pulp]+")
  add_application(testDivRem testDivRem.c LABELS "riscv_tests")
endif()
