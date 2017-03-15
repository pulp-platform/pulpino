set(libs beebs.core)
set(beebs.matmult_SOURCES ../matmult/matmult.c)

add_application(beebs.matmult-float "${beebs.matmult_SOURCES}" FLAGS "-DMATMULT_FLOAT" LABELS "beebs" LIBS "${libs}")
