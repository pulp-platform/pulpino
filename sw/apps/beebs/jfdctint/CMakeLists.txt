set(libs beebs.core)
set(beebs.jfdctint_SOURCES jfdctint.c)

add_application(beebs.jfdctint "${beebs.jfdctint_SOURCES}"
  LABELS "beebs" LIBS "${libs}")
