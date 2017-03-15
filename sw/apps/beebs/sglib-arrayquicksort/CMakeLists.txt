set(libs beebs.core)
set(beebs.arraysort_SOURCES ../sglib-arraysort/arraysort.c)

add_application(beebs.sglib-arrayquicksort "${beebs.arraysort_SOURCES}" FLAGS "-DQUICK_SORT" LABELS "beebs" LIBS "${libs}")
