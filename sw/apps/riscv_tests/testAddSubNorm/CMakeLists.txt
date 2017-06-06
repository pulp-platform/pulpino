if (${GCC_MARCH} MATCHES "IMXpulpv2")
  add_application(testAddSubNorm testAddSubNorm.c LABELS "riscv_tests" "verilator_tests")
endif()
