set(libs beebs.core)
set(beebs.ludcmp_SOURCES ludcmp.c)

add_application(beebs.ludcmp "${beebs.ludcmp_SOURCES}" LABELS "beebs" LIBS "${libs}")
