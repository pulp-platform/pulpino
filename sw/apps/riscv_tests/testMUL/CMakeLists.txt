if (${GCC_MARCH} MATCHES "RV32IM" OR ${GCC_MARCH} MATCHES "[pulp]+")
  add_application(testMUL testMUL.c LABELS "riscv_tests")
endif()
