set(FP_SOURCES mlRbf.c)

if (${RISCY_RV32F})
  add_application(mlRbf "${FP_SOURCES}" LABELS "ml_tests")
endif()
