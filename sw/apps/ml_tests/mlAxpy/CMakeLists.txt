set(FP_SOURCES mlAxpy.c)

if (${RISCY_RV32F})
  add_application(mlAxpy "${FP_SOURCES}" LABELS "ml_tests")
endif()
