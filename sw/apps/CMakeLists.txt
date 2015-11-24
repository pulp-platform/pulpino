set(UTILS_DIR ${CMAKE_SOURCE_DIR}/utils)

include(CMakeSim.txt)

################################################################################
# main application macro
# Optional arguments:
#  - SUBDIR prefix   (prefix application with prefix)
#
# Attention: Every application name has to be unique and must have its own
#            build folder, so if you have multiple applications in one folder,
#            use SUBDIR to separate them
################################################################################
macro(add_application NAME SOURCE_FILES)
  # optional argument parsing
  set(oneValueArgs SUBDIR TB LABELS FLAGS)
  set(multiValueArgs LIBS)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  set(SUBDIR ${ARG_SUBDIR})

  include_directories(
    ${CMAKE_SOURCE_DIR}/libs/malloc_lib/inc
    ${CMAKE_SOURCE_DIR}/libs/string_lib/inc
    ${CMAKE_SOURCE_DIR}/libs/sys_lib/inc
    ${CMAKE_SOURCE_DIR}/libs/bench_lib/inc
  )

  add_executable(${NAME}.elf $<TARGET_OBJECTS:crt0> ${SOURCE_FILES})

  # set subdirectory for add_executable
  if(NOT "${SUBDIR}" STREQUAL "")
    set_target_properties(${NAME}.elf PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SUBDIR})
  endif()

  # set optional compile flags per application
  if(NOT "${ARG_FLAGS}" STREQUAL "")
    set_target_properties(${NAME}.elf PROPERTIES COMPILE_FLAGS ${ARG_FLAGS})
  endif()

  # link libraries
  target_link_libraries(${NAME}.elf ${ARG_LIBS} bench string sys)

  # this specifies the testbench to use for simulation
  if(ARG_TB)
    set_target_properties(${NAME}.elf PROPERTIES TB ${ARG_TB})
  else()
    set_target_properties(${NAME}.elf PROPERTIES TB run.tcl)
  endif()

  add_custom_target(${NAME}.read)
  add_custom_command(TARGET ${NAME}.read
    POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} ${CMAKE_OBJDUMP_FLAGS} $<TARGET_FILE:${NAME}.elf> > ${NAME}.read
    WORKING_DIRECTORY ./${SUBDIR}
    DEPENDS ${NAME}.elf)

  # add everything needed for simulation
  add_sim_targets(${NAME})

  # if there are labels available for testing
  if(ARG_LABELS)
    set_tests_properties(${NAME}.test PROPERTIES LABELS "${ARG_LABELS}")
  endif()

endmacro()

################################################################################
# boot code macro
# Used to generate the boot code for pulpino
# Attention: Every application name has to be unique and must have its own
#            build folder
################################################################################
macro(add_boot_code NAME SOURCE_FILES)
  # optional argument parsing
  set(oneValueArgs SUBDIR TB LABELS FLAGS)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "" ${ARGN})
  set(SUBDIR ${ARG_SUBDIR})

  include_directories(
    ${CMAKE_SOURCE_DIR}/libs/sys_lib/inc
  )

  add_executable(${NAME}.elf $<TARGET_OBJECTS:crt0_boot> ${SOURCE_FILES})

  # set subdirectory for add_executable
  if(NOT "${SUBDIR}" STREQUAL "")
    set_target_properties(${NAME}.elf PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SUBDIR})
  endif()

  # set optional compile flags per application
  if(NOT "${ARG_FLAGS}" STREQUAL "")
    set_target_properties(${NAME}.elf PROPERTIES COMPILE_FLAGS ${ARG_FLAGS})
  endif()

  # link libraries
  target_link_libraries(${NAME}.elf sys_chip)


  # this specifies the testbench to use for simulation
  if(ARG_TB)
    set_target_properties(${NAME}.elf PROPERTIES TB ${ARG_TB})
  else()
    set_target_properties(${NAME}.elf PROPERTIES TB run.tcl)
  endif()

  add_custom_target(${NAME}.read)
  add_custom_command(TARGET ${NAME}.read
    POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} ${CMAKE_OBJDUMP_FLAGS} $<TARGET_FILE:${NAME}.elf> > ${NAME}.read
    WORKING_DIRECTORY ./${SUBDIR}
    DEPENDS ${NAME}.elf)

  # add everything needed for simulation
  add_sim_targets(${NAME})

  # if there are labels available for testing
  if(ARG_LABELS)
    set_tests_properties(${NAME}.test PROPERTIES LABELS "${ARG_LABELS}")
  endif()

endmacro()

################################################################################

# add_subdirectory(spi_regression)

# add_subdirectory(boot_code)

add_subdirectory(helloworld)
add_subdirectory(freertos)

# add_subdirectory(debug)
add_subdirectory(bench)

# RISCV only tests
if(${RISCV})
  #add_subdirectory(compressed)
  add_subdirectory(fpga_test)
  add_subdirectory(riscv_tests)
endif()

add_subdirectory(sequential_tests)
add_subdirectory(imperio_tests)

if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/scratch/")
  add_subdirectory(scratch)
endif()
