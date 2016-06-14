set(SOURCES
    src/wiring_digital.c
    )

set(HEADERS
    inc/main.cpp
    inc/Arduino.h
    inc/pins_arduino.h
    inc/wiring_private.h
    )

include_directories(inc/)
include_directories(../sys_lib/inc)

add_library(Arduino STATIC ${SOURCES} ${HEADERS})


