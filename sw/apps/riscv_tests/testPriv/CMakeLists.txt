if (${GCC_MARCH} MATCHES "[*pulpv2]")
  add_application(testPriv testPriv.c LABELS "riscv_tests")
endif()
