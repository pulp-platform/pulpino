set(SOURCES
    src/fprintf.c
    src/stdio.c
    )

set(HEADERS
    inc/stdio.h
    )

include_directories(inc/)
include_directories(../sys_lib/inc)
include_directories(../string_lib/inc)

add_library(stdio STATIC ${SOURCES} ${HEADERS})
