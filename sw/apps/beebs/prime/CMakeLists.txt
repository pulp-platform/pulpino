set(libs beebs.core)
set(beebs.prime_SOURCES prime.c)

add_application(beebs.prime "${beebs.prime_SOURCES}" LABELS "beebs" LIBS "${libs}")
