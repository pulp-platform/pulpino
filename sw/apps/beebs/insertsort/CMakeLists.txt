set(libs beebs.core)
set(beebs.insertsort_SOURCES insertsort.c)

add_application(beebs.insertsort "${beebs.insertsort_SOURCES}"
  LABELS "beebs" LIBS "${libs}")
