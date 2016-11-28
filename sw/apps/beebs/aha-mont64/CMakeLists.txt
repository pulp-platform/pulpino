set(libs beebs.core)
set(beebs.mont64_SOURCES mont64.c)

add_application(beebs.aha-mont64 "${beebs.mont64_SOURCES}" LABELS "beebs" LIBS "${libs}")
