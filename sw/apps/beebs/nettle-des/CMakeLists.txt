set(libs beebs.core)
set(beebs.nettle_des_SOURCES des.c)

add_application(beebs.nettle-des "${beebs.nettle_des_SOURCES}"
  LABELS "beebs" LIBS "${libs}")
