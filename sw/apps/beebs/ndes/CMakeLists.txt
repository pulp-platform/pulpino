set(libs beebs.core)
set(beebs.ndes_SOURCES ndes.c)

add_application(beebs.ndes "${beebs.ndes_SOURCES}" LABELS "beebs" LIBS "${libs}")
