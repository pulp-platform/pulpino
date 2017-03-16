if (${GCC_ETH})
  add_application(testShufflePack testShufflePack.c LABELS "riscv_tests" "verilator_tests")
endif()
