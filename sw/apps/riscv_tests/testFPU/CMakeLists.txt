set(FP_SOURCES testFPU.c)

if (${RISCY_RV32F})
  add_application(testFPU "${FP_SOURCES}" LABELS "riscv_tests")
endif()
