if (${GCC_ETH})
    set(CONV_SOURCES convolution.c conv_kernels.c)
    add_application(convolution "${CONV_SOURCES}" FLAGS "-DPROFILE" LABELS "sequential_tests" LABELS "verilator_tests")
endif()