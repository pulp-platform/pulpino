if (${GCC_ETH})
add_application(motion_detection motion_detection.c FLAGS LABELS "sequential_tests" "verilator_tests")
endif()
