set(libs beebs.core)
set(beebs.arraybinsearch_SOURCES arraybinsearch.c)

add_application(beebs.sglib-arraybinsearch "${beebs.arraybinsearch_SOURCES}" LABELS "beebs" LIBS "${libs}")
