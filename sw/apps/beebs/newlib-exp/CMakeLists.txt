set(libs beebs.core)
set(beebs.ef_exp_SOURCES ef_exp.c)

add_application(beebs.newlib-exp "${beebs.ef_exp_SOURCES}"
  LABELS "beebs" LIBS "${libs}")
