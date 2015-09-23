set(libs perfbench.core perfbench.extras)
set(perfbench.fir_SOURCES fir.c fir_test.c)

add_application(perfbench.fir "${perfbench.fir_SOURCES}"
  LABELS "perfbench" LIBS "${libs}")
