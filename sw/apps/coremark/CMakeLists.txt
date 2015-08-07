set(CORE_SOURCES pulp/core_portme.c core_list_join.c core_main.c core_matrix.c core_state.c core_util.c)

include_directories(BEFORE "." "./pulp")
add_application(coremark "${CORE_SOURCES}" FLAGS "${CORE_DEFINES}")

# set ITERATIONS to the number of iterations to perform (each one takes about 500'000 cycles)
# set MULTITHREAD to the number of cores that should perform the benchmark in parallel
set(CORE_DEFINES ITERATIONS=1 FLAGS_STR="NOT IMPLEMENTED" MULTITHREAD=1)
set_target_properties(coremark PROPERTIES COMPILE_DEFINITIONS "${CORE_DEFINES}")
