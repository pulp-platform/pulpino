set(libs beebs.core)
set(beebs.whetstone_SOURCES whetstone.c)

add_application(beebs.whetstone "${beebs.whetstone_SOURCES}" LABELS "beebs" LIBS "${libs}")