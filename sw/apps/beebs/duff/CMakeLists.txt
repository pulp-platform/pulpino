set(libs beebs.core)
set(beebs.duff_SOURCES duff.c)

add_application(beebs.duff "${beebs.duff_SOURCES}" LABELS "beebs" LIBS "${libs}")
