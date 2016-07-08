if (${GCC_ETH})
  add_application(testAddSubNorm testAddSubNorm.c LABELS "riscv_tests" "verilator_tests")
endif()
