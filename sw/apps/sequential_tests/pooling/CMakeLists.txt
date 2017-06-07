set(SOURCES pool_kernels.c pooling.c)

if (${GCC_MARCH} MATCHES "IMXpulpv2")
        add_application(pooling "${SOURCES}" FLAGS "-DPROFILE -DVEC" LABELS "sequential_tests" "verilator_tests")
else()
        add_application(pooling "${SOURCES}" FLAGS "-DPROFILE" LABELS "sequential_tests" "verilator_tests")
endif()