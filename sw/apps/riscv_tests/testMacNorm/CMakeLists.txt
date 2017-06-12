if (${GCC_MARCH} MATCHES "[pulp]+")
  add_application(testMacNorm testMacNorm.c LABELS "riscv_tests" "verilator_tests")
endif()
