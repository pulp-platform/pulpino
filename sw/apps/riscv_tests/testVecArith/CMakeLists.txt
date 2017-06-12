if (${GCC_MARCH} MATCHES "[pulp]+")
  add_application(testVecArith testVecArith.c LABELS "riscv_tests" "verilator_tests")
endif()
