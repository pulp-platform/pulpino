if (${GCC_MARCH} MATCHES "IMXpulpv2")
  add_application(testShufflePack testShufflePack.c LABELS "riscv_tests" "verilator_tests")
endif()
