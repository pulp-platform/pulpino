if (${GCC_MARCH} MATCHES "[pulp]+")
  add_application(testBitManipulation testBitManipulation.c LABELS "riscv_tests")
endif()
