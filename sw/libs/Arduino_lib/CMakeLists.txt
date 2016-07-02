set(SOURCES
    src/wiring_digital.c
    src/WInterrupts.c
    src/hooks.c
    src/wiring.c
    src/wiring_analog.c
    src/WString.cpp
    )

set(HEADERS
    inc/main.cpp
    inc/Arduino.h
    inc/pins_arduino.h
    inc/wiring_private.h
    inc/WString.h
    )

include_directories(inc/)
include_directories(../sys_lib/inc)

add_library(Arduino STATIC ${SOURCES} ${HEADERS})


