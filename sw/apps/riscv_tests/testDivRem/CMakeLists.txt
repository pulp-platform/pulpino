if (${GCC_ETH})
  add_application(testDivRem testDivRem.c LABELS "riscv_tests" "verilator_tests")
endif()
