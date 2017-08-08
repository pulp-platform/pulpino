if (${GCC_MARCH} MATCHES "[pulp]+")
  add_application(testAddSubNorm testAddSubNorm.c LABELS "riscv_tests")
endif()
