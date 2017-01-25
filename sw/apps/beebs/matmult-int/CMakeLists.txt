set(libs beebs.core)
set(beebs.matmult_SOURCES ../matmult/matmult.c)

add_application(beebs.matmult-int "${beebs.matmult_SOURCES}" FLAGS "-DMATMULT_INT" LABELS "beebs" LIBS "${libs}")
