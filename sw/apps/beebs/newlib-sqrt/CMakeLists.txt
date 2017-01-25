set(libs beebs.core)
set(beebs.ef_sqrt_SOURCES ef_sqrt.c)

add_application(beebs.newlib-sqrt "${beebs.ef_sqrt_SOURCES}"
  LABELS "beebs" LIBS "${libs}")
