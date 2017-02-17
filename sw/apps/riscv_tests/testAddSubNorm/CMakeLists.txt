if (${GCC_ETH} AND NOT${GCC_ETH_SLIM})
  add_application(testAddSubNorm testAddSubNorm.c LABELS "riscv_tests")
endif()
