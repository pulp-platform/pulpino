set(FP_SOURCES mlBilat.c)

if (${RISCY_RV32F})
  add_application(mlBilat "${FP_SOURCES}" LABELS "ml_tests")
endif()
