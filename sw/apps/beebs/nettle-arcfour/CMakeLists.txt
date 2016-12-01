set(libs beebs.core)
set(beebs.arcfour_SOURCES arcfour.c)

add_application(beebs.nettle-arcfour "${beebs.arcfour_SOURCES}" LABELS "beebs" LIBS "${libs}")
