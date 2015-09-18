set(FREERTOS_SOURCES main.c Source/list.c Source/queue.c Source/tasks.c Source/portable/GCC/RI5CY/port.c Source/portable/MemMang/heap_1.c)

include_directories(. Source/portable/GCC/RI5CY Source/include)

add_application(freertos "${FREERTOS_SOURCES}")
