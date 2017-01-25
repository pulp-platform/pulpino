set(libs beebs.core)
set(beebs.fir_SOURCES fir.c)

add_application(beebs.fir "${beebs.fir_SOURCES}"
  LABELS "beebs" LIBS "${libs}")