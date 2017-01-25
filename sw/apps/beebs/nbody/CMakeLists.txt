set(libs beebs.core)
set(beebs.nbody_SOURCES nbody.c)

add_application(beebs.nbody "${beebs.nbody_SOURCES}"
  LABELS "beebs" LIBS "${libs}")
