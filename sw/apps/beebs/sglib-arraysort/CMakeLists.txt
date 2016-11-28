set(libs beebs.core)
set(beebs.arraysort_SOURCES arraysort.c)

add_application(beebs.sglib-arraysort "${beebs.arraysort_SOURCES}"
  FLAGS "-DQUICK_SORT" LABELS "beebs" LIBS "${libs}")
