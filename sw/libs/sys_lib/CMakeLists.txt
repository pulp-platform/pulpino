set(SOURCES
    src/bbmux_clkgate.c
    src/clock.c
    src/cvp.c
    src/events.c
    src/exceptions.c
    src/gpio.c
    src/hwce.c
    src/icache_ctrl.c
    src/int.c
    src/pipe_stage.c
    src/spi.c
    src/timer.c
    src/uart.c
    src/utils.c
    )

set(HEADERS
    inc/bar.h
    inc/bbmux_clkgate.h
    inc/clock.h
    inc/cvp.h
    inc/events.h
    inc/events_ids.h
    inc/gpio.h
    inc/hwce.h
    inc/icache_ctrl.h
    inc/int.h
    inc/mchan.h
    inc/pipe_stage.h
    inc/pulpemu.h
    inc/pulp.h
    inc/spi.h
    inc/spr-defs.h
    inc/timer.h
    inc/uart.h
    inc/utils.h
    )

include_directories(inc/)
include_directories(../string_lib/inc)

add_library(sys STATIC ${SOURCES} ${HEADERS})
add_library(sys_chip STATIC ${SOURCES} ${HEADERS})
set_target_properties(sys_chip PROPERTIES COMPILE_FLAGS "-DCHIP_USED")
