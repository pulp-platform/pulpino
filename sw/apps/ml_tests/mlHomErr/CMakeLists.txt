set(FP_SOURCES mlHomErr.c)

if (${RISCY_RV32F})
  add_application(mlHomErr "${FP_SOURCES}" LABELS "ml_tests")
endif()
