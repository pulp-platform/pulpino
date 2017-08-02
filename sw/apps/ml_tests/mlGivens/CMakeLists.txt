set(FP_SOURCES mlGivens.c)

if (${RISCY_RV32F})
  add_application(mlGivens "${FP_SOURCES}" LABELS "ml_tests")
endif()
