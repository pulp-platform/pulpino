if (${USE_ZERO_RISCY})
    add_application(testMUL testMUL.c FLAGS "-DUSE_ZERO_RISCY" LABELS "riscv_tests")
else()
    add_application(testMUL testMUL.c LABELS "riscv_tests")
endif()

