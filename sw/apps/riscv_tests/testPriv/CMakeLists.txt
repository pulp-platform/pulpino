if (${GCC_MARCH} MATCHES "[pulp]+")
  add_application(testPriv testPriv.c LABELS "riscv_tests")
endif()
