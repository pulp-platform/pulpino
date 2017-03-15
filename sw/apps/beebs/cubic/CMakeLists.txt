set(libs beebs.core)
set(beebs.cubic_SOURCES cubic.c basicmath_small.c)

add_application(beebs.cubic "${beebs.cubic_SOURCES}" LABELS "beebs" LIBS "${libs}")
