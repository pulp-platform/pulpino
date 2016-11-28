set(libs beebs.core)
set(beebs.cover_SOURCES cover.c)

add_application(beebs.cover "${beebs.cover_SOURCES}" LABELS "beebs" LIBS "${libs}")
