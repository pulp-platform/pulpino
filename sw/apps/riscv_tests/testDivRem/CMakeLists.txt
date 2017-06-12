if (${GCC_MARCH} MATCHES "RV32IM" OR ${GCC_MARCH} MATCHES "[*pulpv2]")
  add_application(testDivRem testDivRem.c LABELS "riscv_tests" "verilator_tests")
endif()
