set(FP_SOURCES mlSchur.c)

if (${RISCY_RV32F})
  add_application(mlSchur "${FP_SOURCES}" LABELS "ml_tests")
endif()
