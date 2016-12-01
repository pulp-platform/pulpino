set(libs beebs.core)
set(beebs.minver_SOURCES minver.c)

add_application(beebs.minver "${beebs.minver_SOURCES}" LABELS "beebs" LIBS "${libs}")
