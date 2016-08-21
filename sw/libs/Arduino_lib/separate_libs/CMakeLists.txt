set(SOURCES
    src/twi.c
    src/Wire.cpp
    src/SPI.cpp
    )

set(HEADERS
    inc/twi.h
    inc/Wire.h
    inc/SPI.h
    )

include_directories(inc/)
include_directories(../core_libs/inc)
include_directories(../../sys_lib/inc)

add_library(Arduino_separate STATIC ${SOURCES} ${HEADERS})


