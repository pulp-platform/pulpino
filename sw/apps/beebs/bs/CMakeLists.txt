set(libs beebs.core)
set(beebs.bs_SOURCES bs.c)

add_application(beebs.bs "${beebs.bs_SOURCES}"
  LABELS "beebs" LIBS "${libs}")
