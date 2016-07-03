set(SOURCES
    src/wiring_digital.c
    src/WInterrupts.c
    src/hooks.c
    src/wiring.c
    src/wiring_analog.c
    src/WString.cpp
    src/Print.cpp
    src/Stream.cpp
    )

set(HEADERS
    inc/main.cpp
    inc/Arduino.h
    inc/pins_arduino.h
    inc/wiring_private.h
    inc/WString.h
    inc/Printable.h
    inc/Print.h
    inc/Stream.h
    inc/HardwareSerial_private.h
    inc/HardwareSerial.h
    )

include_directories(inc/)
include_directories(../sys_lib/inc)

add_library(Arduino STATIC ${SOURCES} ${HEADERS})


