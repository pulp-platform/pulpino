set(libs beebs.core)
set(beebs.ud_SOURCES ud.c)

add_application(beebs.ud "${beebs.ud_SOURCES}"
  LABELS "beebs" LIBS "${libs}")
