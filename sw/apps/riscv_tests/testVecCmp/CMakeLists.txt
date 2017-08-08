if (${GCC_MARCH} MATCHES "[pulp]+")
  add_application(testVecCmp testVecCmp.c LABELS "riscv_tests")
endif()
