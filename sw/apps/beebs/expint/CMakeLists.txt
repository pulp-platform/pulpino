set(libs beebs.core)
set(beebs.expint_SOURCES expint.c)

add_application(beebs.expint "${beebs.expint_SOURCES}"
  LABELS "beebs" LIBS "${libs}")
