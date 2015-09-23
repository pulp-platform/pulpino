set(perfbench.ipm_SOURCES ipm.c ipm_test.c)
add_application(perfbench.ipm "${perfbench.ipm_SOURCES}"
  LABELS "perfbench" LIBS perfbench.core)
