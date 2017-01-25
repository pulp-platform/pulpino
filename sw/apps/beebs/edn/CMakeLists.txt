set(libs beebs.core)
set(beebs.edn_SOURCES edn.c)

add_application(beebs.edn "${beebs.edn_SOURCES}" LABELS "beebs" LIBS "${libs}")
