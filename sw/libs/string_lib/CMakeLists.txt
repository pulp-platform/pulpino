set(SOURCES
    src/qprintf.c
    )

set(HEADERS
    inc/string_lib.h
    )

include_directories(inc/)
include_directories(../sys_lib/inc)

add_library(string STATIC ${SOURCES} ${HEADERS})
add_library(string_uart STATIC ${SOURCES} ${HEADERS})
set_target_properties(string_uart PROPERTIES COMPILE_FLAGS "-DPRINTF_USE_UART")
