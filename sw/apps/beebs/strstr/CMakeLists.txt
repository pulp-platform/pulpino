set(libs beebs.core)
set(beebs.strstr_SOURCES strstr.c)

add_application(beebs.strstr "${beebs.strstr_SOURCES}" LABELS "beebs" LIBS "${libs}")