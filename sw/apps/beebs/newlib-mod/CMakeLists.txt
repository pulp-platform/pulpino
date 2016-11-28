set(libs beebs.core)
set(beebs.ef_mod_SOURCES ef_mod.c)

add_application(beebs.newlib-mod "${beebs.ef_mod_SOURCES}"
  LABELS "beebs" LIBS "${libs}")
