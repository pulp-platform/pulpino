set(libs beebs.core)
set(beebs.cnt_SOURCES cnt.c)

add_application(beebs.cnt "${beebs.cnt_SOURCES}" LABELS "beebs" LIBS "${libs}")
