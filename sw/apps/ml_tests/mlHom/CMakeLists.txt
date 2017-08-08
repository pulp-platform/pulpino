set(FP_SOURCES mlHom.c)

if (${RISCY_RV32F})
  add_application(mlHom "${FP_SOURCES}" LABELS "ml_tests")
endif()
