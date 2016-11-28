set(libs beebs.core)
set(beebs.fibcall_SOURCES fibcall.c)

add_application(beebs.fibcall "${beebs.fibcall_SOURCES}"
  LABELS "beebs" LIBS "${libs}")
