set(FP_SOURCES mlDist.c)

if (${RISCY_RV32F})
  add_application(mlDist "${FP_SOURCES}" LABELS "ml_tests")
endif()
