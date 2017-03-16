if (${GCC_ETH})
  add_application(testVecCmp testVecCmp.c LABELS "riscv_tests" "verilator_tests")
endif()