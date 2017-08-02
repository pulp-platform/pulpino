set(FP_SOURCES mlGemv.c)

if (${RISCY_RV32F})
  add_application(mlGemv "${FP_SOURCES}" LABELS "ml_tests")
endif()
