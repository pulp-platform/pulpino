set(libs beebs.core)
set(beebs.ef_log_SOURCES ef_log.c)

add_application(beebs.newlib-log "${beebs.ef_log_SOURCES}"
  LABELS "beebs" LIBS "${libs}")
