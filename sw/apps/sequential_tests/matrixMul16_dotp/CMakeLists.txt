set(MATMUL_SOURCES matrixMul.c matmul_kernels.c)

if (${GCC_MARCH} MATCHES "IMXpulpv2")
        add_application(matrixMul16_dotp "${MATMUL_SOURCES}" FLAGS "-DUSE_VEC" "sequential_tests")
else()
        add_application(matrixMul16_dotp "${MATMUL_SOURCES}" FLAGS LABELS "sequential_tests")
endif()
