if (${GCC_MARCH} MATCHES "[pulp]+")
  add_application(testShufflePack testShufflePack.c LABELS "riscv_tests")
endif()
