if (${GCC_ETH})
    set(SOURCES pool_kernels.c pooling.c)
    add_application(pooling "${SOURCES}" FLAGS "-DPROFILE" LABELS "sequential_tests" "verilator_tests")
endif()