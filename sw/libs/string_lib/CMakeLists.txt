set(SOURCES
    src/qprintf.c
    )

set(HEADERS
    inc/string_lib.h
    )

include_directories(inc/)
include_directories(../sys_lib/inc)

add_library(string STATIC ${SOURCES} ${HEADERS})
