set(libs beebs.core)
set(beebs.arraysort_SOURCES ../sglib-arraysort/arraysort.c)

add_application(beebs.sglib-arrayheapsort "${beebs.arraysort_SOURCES}" FLAGS "-DHEAP_SORT" LABELS "beebs" LIBS "${libs}")
