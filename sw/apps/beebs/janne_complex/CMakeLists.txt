set(libs beebs.core)
set(beebs.janne_complex_SOURCES janne_complex.c)

add_application(beebs.janne_complex "${beebs.janne_complex_SOURCES}" LABELS "beebs" LIBS "${libs}")
