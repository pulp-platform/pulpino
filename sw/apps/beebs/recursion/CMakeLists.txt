set(libs beebs.core)
set(beebs.recursion_SOURCES recursion.c)

add_application(beebs.recursion "${beebs.recursion_SOURCES}" LABELS "beebs" LIBS "${libs}")
