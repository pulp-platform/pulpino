set(FP_SOURCES mlWdotp.c)

if (${RISCY_RV32F})
  add_application(mlWdotp "${FP_SOURCES}" LABELS "ml_tests")
endif()
