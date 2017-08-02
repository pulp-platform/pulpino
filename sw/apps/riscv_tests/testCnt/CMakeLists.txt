if (${GCC_MARCH} MATCHES "[pulp]+")
  add_application(testCnt testCnt.c LABELS "riscv_tests")
endif()
