set(MATMUL_SOURCES matrixMul.c matmul_kernels.c)

if (${GCC_MARCH} MATCHES "IMXpulpv2")
    add_application(matrixMul8_dotp "${MATMUL_SOURCES}" FLAGS "-DUSE_VEC" "sequential_tests" "verilator_tests")
else()
    add_application(matrixMul8_dotp "${MATMUL_SOURCES}" FLAGS LABELS "sequential_tests" "verilator_tests")
endif()
