if (${GCC_ETH})
  add_application(testShufflePack testShufflePack.c LABELS "riscv_tests" LABELS "verilator_tests")
endif()
