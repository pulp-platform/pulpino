set(MATMUL_SOURCES matrixMul.c matmul_kernels.c)

if (${GCC_MARCH} MATCHES "[pulp]+")
    add_application(matrixMul8_dotp "${MATMUL_SOURCES}" FLAGS "-DUSE_VEC" LABELS "sequential_tests")
else()
    add_application(matrixMul8_dotp "${MATMUL_SOURCES}" FLAGS LABELS "sequential_tests")
endif()
