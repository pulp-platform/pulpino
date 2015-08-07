set(SOURCES
    src/bench.c
    )

set(HEADERS
    inc/bench.h
    )

include_directories(inc/)
include_directories(../string_lib/inc)

add_library(bench STATIC ${SOURCES} ${HEADERS})
