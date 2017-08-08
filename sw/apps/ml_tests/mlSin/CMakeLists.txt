set(FP_SOURCES mlSin.c)

if (${RISCY_RV32F})
  add_application(mlSin "${FP_SOURCES}" LABELS "ml_tests")
endif()
