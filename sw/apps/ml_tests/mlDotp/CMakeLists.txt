set(FP_SOURCES mlDotp.c)

if (${RISCY_RV32F})
  add_application(mlDotp "${FP_SOURCES}" LABELS "ml_tests")
endif()
