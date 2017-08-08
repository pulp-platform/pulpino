if (${GCC_MARCH} MATCHES "[pulp]+")
  add_application(testClip testClip.c LABELS "riscv_tests")
endif()
