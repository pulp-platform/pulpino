set(libs beebs.core)
set(beebs.qurt_SOURCES qurt.c)

add_application(beebs.qurt "${beebs.qurt_SOURCES}" LABELS "beebs" LIBS "${libs}")
