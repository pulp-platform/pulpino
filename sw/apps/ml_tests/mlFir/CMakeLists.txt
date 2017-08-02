set(FP_SOURCES mlFir.c)

if (${RISCY_RV32F})
  add_application(mlFir "${FP_SOURCES}" LABELS "ml_tests")
endif()
