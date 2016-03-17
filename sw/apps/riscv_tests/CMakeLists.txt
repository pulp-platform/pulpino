include_directories(includes)

add_subdirectory(basic)
add_subdirectory(official)

add_subdirectory(testBitManipulation)
add_subdirectory(testCnt)

# memory access
add_subdirectory(testMisaligned)

# interrupts and events
# add_subdirectory(testEvents)
add_subdirectory(testExceptions)

# arithmetic operations
add_subdirectory(testALU)
add_subdirectory(testDivRem)
add_subdirectory(testShufflePack)

# other core features
add_subdirectory(testCSR)

# add_subdirectory(testDebug)

add_subdirectory(debug)
