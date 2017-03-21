if (${GCC_ETH})
    set(CNN_SOURCES cnn_layer.c cnn_kernels.c cnn_kernels_fused.c)
    add_application(cnn_layer "${CNN_SOURCES}" FLAGS "-DPROFILE" LABELS "sequential_tests" "verilator_tests")
endif()