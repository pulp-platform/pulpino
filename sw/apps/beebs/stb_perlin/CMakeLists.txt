set(libs beebs.core)
set(beebs.stb_perlin_SOURCES stb_perlin.c)

add_application(beebs.stb_perlin "${beebs.stb_perlin_SOURCES}" LABELS "beebs" LIBS "${libs}")