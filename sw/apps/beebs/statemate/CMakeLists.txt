set(libs beebs.core)
set(beebs.statemate_SOURCES statemate.c)

add_application(beebs.statemate "${beebs.statemate_SOURCES}" LABELS "beebs" LIBS "${libs}")