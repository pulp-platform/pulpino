set(FP_SOURCES mlGemm.c)

if (${RISCY_RV32F})
  add_application(mlGemm "${FP_SOURCES}" LABELS "ml_tests")
endif()
