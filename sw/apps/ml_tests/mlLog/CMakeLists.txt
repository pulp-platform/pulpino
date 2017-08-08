set(FP_SOURCES mlLog.c)

if (${RISCY_RV32F})
  add_application(mlLog "${FP_SOURCES}" LABELS "ml_tests")
endif()
