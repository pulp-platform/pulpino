set(libs beebs.core)
set(beebs.sqrt_SOURCES sqrt.c)

add_application(beebs.sqrt "${beebs.sqrt_SOURCES}"
  LABELS "beebs" LIBS "${libs}")