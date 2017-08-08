set(FP_SOURCES mlDct.c)

if (${RISCY_RV32F})
  add_application(mlDct "${FP_SOURCES}" LABELS "ml_tests")
endif()
