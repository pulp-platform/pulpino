set(libs beebs.core)
set(beebs.compress_SOURCES compress_test.c)

add_application(beebs.aha-compress "${beebs.compress_SOURCES}" LABELS "beebs" LIBS "${libs}")
