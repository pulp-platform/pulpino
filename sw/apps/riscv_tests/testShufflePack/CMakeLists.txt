if (${GCC_MARCH} MATCHES "[*pulpv2]")
  add_application(testShufflePack testShufflePack.c LABELS "riscv_tests" "verilator_tests")
endif()
