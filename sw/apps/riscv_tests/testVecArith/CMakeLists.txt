if (${GCC_MARCH} MATCHES "IMXpulpv2")
  add_application(testVecArith testVecArith.c LABELS "riscv_tests" "verilator_tests")
endif()
