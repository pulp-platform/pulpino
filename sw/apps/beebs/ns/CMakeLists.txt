set(libs beebs.core)
set(beebs.ns_SOURCES ns.c)

add_application(beebs.ns "${beebs.ns_SOURCES}" LABELS "beebs" LIBS "${libs}")
