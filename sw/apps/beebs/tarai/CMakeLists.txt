set(libs beebs.core)
set(beebs.tarai_SOURCES tarai.c)

add_application(beebs.tarai "${beebs.tarai_SOURCES}" LABELS "beebs" LIBS "${libs}")