set(libs beebs.core)
set(beebs.bubblesort_SOURCES bubblesort.c)

add_application(beebs.bubblesort "${beebs.bubblesort_SOURCES}" LABELS "beebs" LIBS "${libs}")
