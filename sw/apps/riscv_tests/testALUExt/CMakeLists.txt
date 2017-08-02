if (${GCC_MARCH} MATCHES "[pulp]+")
  add_application(testALUExt testALUExt.c LABELS "riscv_tests")
endif()
