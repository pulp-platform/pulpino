set(SOURCES
    src/math_fns.c
    )

set(HEADERS
    inc/math_fns.h
    )

include_directories(inc/)

add_library(math_fns STATIC ${SOURCES} ${HEADERS})
