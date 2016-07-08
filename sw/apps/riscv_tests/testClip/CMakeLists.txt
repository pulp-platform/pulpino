if (${GCC_ETH})
  add_application(testClip testClip.c LABELS "riscv_tests" "verilator_tests")
endif()
