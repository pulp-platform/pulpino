set(FP_SOURCES mlChol.c)

if (${RISCY_RV32F})
  add_application(mlChol "${FP_SOURCES}" LABELS "ml_tests")
endif()
