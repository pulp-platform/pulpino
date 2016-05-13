set(MATMUL_SOURCES matrixMul.c matmul_kernels.c)

add_application(matrixMul8_dotp "${MATMUL_SOURCES}" FLAGS LABELS "sequential_tests")
