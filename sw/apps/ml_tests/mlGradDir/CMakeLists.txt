set(FP_SOURCES mlGradDir.c)

if (${RISCY_RV32F})
  add_application(mlGradDir "${FP_SOURCES}" LABELS "ml_tests")
endif()
