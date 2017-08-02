set(FP_SOURCES mlGrad.c)

if (${RISCY_RV32F})
  add_application(mlGrad "${FP_SOURCES}" LABELS "ml_tests")
endif()
