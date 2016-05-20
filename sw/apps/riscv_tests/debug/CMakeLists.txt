if (${GCC_ETH})
add_application(debug main.c TB run_debug.tcl LABELS "riscv_tests")
endif()
