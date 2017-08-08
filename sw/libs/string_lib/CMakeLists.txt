set(SOURCES
    src/qprintf.c
    )

set(HEADERS
    inc/string_lib.h
    )

include_directories(inc/)
include_directories(../sys_lib/inc)

add_library(string STATIC ${SOURCES} ${HEADERS})
#set_target_properties(string PROPERTIES COMPILE_FLAGS "-DPOWER_MES -fno-tree-loop-distribute-patterns")
set_target_properties(string PROPERTIES COMPILE_FLAGS "-fno-tree-loop-distribute-patterns")
