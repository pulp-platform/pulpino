if (${GCC_MARCH} MATCHES "IMXpulpv2")
  add_application(testMacNorm testMacNorm.c LABELS "riscv_tests" "verilator_tests")
endif()
