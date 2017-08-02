set(FP_SOURCES mlButter.c)

if (${RISCY_RV32F})
  add_application(mlButter "${FP_SOURCES}" LABELS "ml_tests")
endif()
