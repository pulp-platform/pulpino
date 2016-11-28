set(libs beebs.core)
set(beebs.qsort_SOURCES qsort.c)

add_application(beebs.qsort "${beebs.qsort_SOURCES}"
  LABELS "beebs" LIBS "${libs}")
