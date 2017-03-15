set(libs beebs.core)
set(beebs.slre_SOURCES slre.c)

add_application(beebs.slre "${beebs.slre_SOURCES}" LABELS "beebs" LIBS "${libs}")