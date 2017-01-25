set(libs beebs.core)
set(beebs.fac_SOURCES fac.c)

add_application(beebs.fac "${beebs.fac_SOURCES}" LABELS "beebs" LIBS "${libs}")
