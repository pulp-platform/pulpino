set(libs beebs.core)
set(beebs.fdct_SOURCES fdct.c)

add_application(beebs.fdct "${beebs.fdct_SOURCES}" LABELS "beebs" LIBS "${libs}")
