set(libs beebs.core)
set(beebs.compress_SOURCES compress.c)

add_application(beebs.compress "${beebs.compress_SOURCES}" LABELS "beebs" LIBS "${libs}")
