include_directories(includes)

add_subdirectory(basic)
add_subdirectory(official)

# memory access
# add_subdirectory(testLoadStore)
# add_subdirectory(testMisaligned)

# interrupts and events
# add_subdirectory(testEvents)
# add_subdirectory(testExceptions)

# arithmetic operations
# add_subdirectory(testALU)
# add_subdirectory(testALUExt)
# add_subdirectory(testCnt)
# add_subdirectory(testMAC)

# other core features
# add_subdirectory(testCSR)
# add_subdirectory(testDebug)

add_subdirectory(debug)
