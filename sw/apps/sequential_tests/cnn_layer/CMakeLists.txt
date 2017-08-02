set(CNN_SOURCES cnn_layer.c cnn_kernels.c cnn_kernels_fused.c)

if (${GCC_MARCH} MATCHES "[pulp]+")
        add_application(cnn_layer "${CNN_SOURCES}" FLAGS "-DPROFILE -DVEC" LABELS "sequential_tests")
else()
        add_application(cnn_layer "${CNN_SOURCES}" FLAGS "-DPROFILE" LABELS "sequential_tests")
endif()
