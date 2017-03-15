set(CORE_SOURCES dhry_1.c dhry_2.c)
add_application(dhrystone "${CORE_SOURCES}" LABELS "dhrystone")

set(CORE_DEFINES DEF_CUSTOM_NRUNS CUSTOM_REG CUSTOM_NRUNS=500 CUSTOM_TIME)

set_target_properties(dhrystone.elf PROPERTIES COMPILE_DEFINITIONS "${CORE_DEFINES}")