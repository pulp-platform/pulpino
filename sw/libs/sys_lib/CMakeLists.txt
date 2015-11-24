set(SOURCES
    src/exceptions.c
    src/gpio.c
    src/int.c
    src/spi.c
    src/timer.c
    src/uart.c
    src/utils.c
    src/i2c.c
    )

set(HEADERS
    inc/bar.h
    inc/gpio.h
    inc/int.h
    inc/pulpino.h
    inc/spi.h
    inc/spr-defs.h
    inc/timer.h
    inc/uart.h
    inc/utils.h
    inc/i2c.h
    )

include_directories(inc/)
include_directories(../string_lib/inc)

add_library(sys STATIC ${SOURCES} ${HEADERS})
