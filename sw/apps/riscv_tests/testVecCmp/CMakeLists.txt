if (${GCC_MARCH} MATCHES "[*pulpv2]")
  add_application(testVecCmp testVecCmp.c LABELS "riscv_tests" "verilator_tests")
endif()
