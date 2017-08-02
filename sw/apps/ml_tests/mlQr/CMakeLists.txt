set(FP_SOURCES mlQr.c)

if (${RISCY_RV32F})
  add_application(mlQr "${FP_SOURCES}" LABELS "ml_tests")
endif()
