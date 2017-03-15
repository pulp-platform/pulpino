set(libs beebs.core)
set(beebs.frac_SOURCES frac.c)

add_application(beebs.frac "${beebs.frac_SOURCES}" LABELS "beebs" LIBS "${libs}")
