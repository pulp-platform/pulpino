set(libs beebs.core)
set(beebs.select_SOURCES select.c)

add_application(beebs.select "${beebs.select_SOURCES}" LABELS "beebs" LIBS "${libs}")
