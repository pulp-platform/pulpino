set(SOURCES
    src/boardsupport.c
    )

set(HEADERS
    inc/support.h
    inc/boardsupport.h
    )

include_directories(inc/)
include_directories(../sys_lib/inc)

add_library(beebs STATIC ${SOURCES} ${HEADERS})
