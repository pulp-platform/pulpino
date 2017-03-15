set(libs beebs.core)
set(beebs.cast128_SOURCES cast128.c)

add_application(beebs.nettle-cast128 "${beebs.cast128_SOURCES}"
  LABELS "beebs" LIBS "${libs}")
