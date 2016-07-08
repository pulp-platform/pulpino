if (${GCC_ETH})
  add_application(testCnt testCnt.c LABELS "riscv_tests" "verilator_tests")
endif()
