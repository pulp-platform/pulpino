if (${GCC_ETH})
add_application(testIRQ main.c TB run_debug_irq.tcl LABELS "riscv_tests")
endif()
