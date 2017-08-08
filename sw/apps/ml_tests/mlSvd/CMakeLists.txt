set(FP_SOURCES mlSvd.c)

if (${RISCY_RV32F})
  add_application(mlSvd "${FP_SOURCES}" LABELS "ml_tests")
endif()
