if (${GCC_ETH})
  add_application(testVecArith testVecArith.c LABELS "riscv_tests" LABELS "verilator_tests")
endif()
