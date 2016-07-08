if (${GCC_ETH})
  add_application(testMacNorm testMacNorm.c LABELS "riscv_tests" LABELS "verilator_tests")
endif()
