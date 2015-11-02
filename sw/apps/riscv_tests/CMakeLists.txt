include_directories(includes)

add_subdirectory(basic)
add_subdirectory(official)

# memory access
add_subdirectory(testMisaligned)

# interrupts and events
# add_subdirectory(testEvents)
add_subdirectory(testExceptions)

# arithmetic operations
add_subdirectory(testALU)

# other core features
add_subdirectory(testCSR)

# add_subdirectory(testDebug)

add_subdirectory(debug)
