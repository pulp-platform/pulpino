
set(UTILS_DIR ${CMAKE_SOURCE_DIR}/utils)

# main application macro
# Optional arguments:
#  - UART                (use UART instead of fake stdout)
#  - SUBDIR prefix   (prefix application with prefix)
#
# Attention: Every application name has to be unique and must have its own
#            build folder, so if you have multiple applications in one folder,
#            use SUBDIR to separate them
macro(add_application NAME SOURCE_FILES)
  # optional argument parsing
  set(options UART)
  set(oneValueArgs SUBDIR TB LABELS FLAGS)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "" ${ARGN})
  set(SUBDIR ${ARG_SUBDIR})

  include_directories(
    ${CMAKE_SOURCE_DIR}/libs/malloc_lib/inc
    ${CMAKE_SOURCE_DIR}/libs/stdio_lib/inc
    ${CMAKE_SOURCE_DIR}/libs/string_lib/inc
    ${CMAKE_SOURCE_DIR}/libs/sys_lib/inc
    ${CMAKE_SOURCE_DIR}/libs/bench_lib/inc
  )

  add_executable(${NAME} $<TARGET_OBJECTS:crt0> ${SOURCE_FILES})

  # set subdirectory for add_executable
  if(NOT "${SUBDIR}" STREQUAL "")
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SUBDIR})
  endif()

  # set optional compile flags per application
  if(NOT "${ARG_FLAGS}" STREQUAL "")
    set_target_properties(${NAME} PROPERTIES COMPILE_FLAGS ${ARG_FLAGS})
  endif()

  # link libraries
  target_link_libraries(${NAME} bench stdio string_uart sys_chip)


  # this specifies the testbench to use for simulation
  if(ARG_TB)
    set_target_properties(${NAME} PROPERTIES TB ${ARG_TB})
  else()
    set_target_properties(${NAME} PROPERTIES TB run.tcl)
  endif()


  # generate s19 file
  add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/${NAME}.s19
    COMMAND ${CMAKE_OBJCOPY} --srec-len 1 --output-target=srec $<TARGET_FILE:${NAME}> ${NAME}.s19
    WORKING_DIRECTORY ./${SUBDIR}
    DEPENDS ${NAME})
  add_custom_target(${NAME}.s19.cmd ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/${NAME}.s19)

  add_custom_target(${NAME}.read)
  add_custom_command(TARGET ${NAME}.read
    POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -arch=or1kle -mattr=mac3,mul16,lv32,crtl -mcpu=pulp3 -symbolize -print-imm-hex -show-hr-enc -disassemble -section-headers -r -s $<TARGET_FILE:${NAME}> > ${NAME}.read
    WORKING_DIRECTORY ./${SUBDIR}
    DEPENDS ${NAME})

  # generate stripped binary as in good old common.mk
  add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/${NAME}.bin
    COMMAND ${CMAKE_OBJCOPY} -R .debug_frame -R .comment -R .stack -R .heapsram -R .heapscm -O binary $<TARGET_FILE:${NAME}> ${NAME}.bin
    WORKING_DIRECTORY ./${SUBDIR}
    DEPENDS ${NAME})
  add_custom_target(${NAME}.bin.cmd ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/${NAME}.bin)

  ##############################################################################
  # link do_files, tcl_files, work and modelsim.ini from real modelsim
  # directory
  ##############################################################################

  add_custom_command(OUTPUT                                                            ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/work
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${PULP_MODELSIM_DIRECTORY}/work         ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/work)

  add_custom_command(OUTPUT                                                            ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/boot
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${PULP_MODELSIM_DIRECTORY}/boot         ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/boot)

  add_custom_command(OUTPUT                                                            ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/modelsim.ini
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${PULP_MODELSIM_DIRECTORY}/modelsim.ini ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/modelsim.ini)

  add_custom_command(OUTPUT                                                            ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/tcl_files
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${PULP_MODELSIM_DIRECTORY}/tcl_files    ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/tcl_files)

  add_custom_command(OUTPUT                                                            ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/do_files
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${PULP_MODELSIM_DIRECTORY}/do_files     ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/do_files)

  add_custom_target(${NAME}.links ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/modelsim.ini
                                              ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/tcl_files
                                              ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/boot
                                              ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/work
                                              ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/do_files)

  ##############################################################################
  # Generate stimuli
  ##############################################################################

  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/slm_files/)
  add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/slm_files/l2_ram.slm
    COMMAND ${UTILS_DIR}/s19toslm.py ../${NAME}.s19
    COMMAND ${CMAKE_COMMAND} -E touch l2_ram.slm
    WORKING_DIRECTORY ./${SUBDIR}/slm_files/
    DEPENDS ${NAME}.links ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/${NAME}.s19)
  add_custom_target(${NAME}.slm.cmd ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/slm_files/l2_ram.slm)


  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/vectors/)
  add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/vectors/stim.txt
    COMMAND ${UTILS_DIR}/parse_s19.pl ./${NAME}.s19 > ./vectors/stim.txt
    WORKING_DIRECTORY ./${SUBDIR}
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/${NAME}.s19)
  add_custom_target(${NAME}.stim.txt ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/vectors/stim.txt)


  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/stdout/)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR}/FS/)

  # run in modelsim with GUI
  add_custom_target(${NAME}_vsim
    COMMAND ${CMAKE_COMMAND} -E remove stdout/*
    COMMAND ${CMAKE_COMMAND} -E remove FS/*
    COMMAND ${VSIM} -64 -do 'source tcl_files/$<TARGET_PROPERTY:${NAME},TB>\\;'
    WORKING_DIRECTORY ./${SUBDIR}
    DEPENDS ${NAME}.slm.cmd ${NAME}.stim.txt
    COMMENT "Running ${NAME} in ModelSim"
    USES_TERMINAL)

  # run in modelsim in batch mode, output is only generated after simulation terminates
  add_custom_target(${NAME}_vsim_batch
    COMMAND ${CMAKE_COMMAND} -E remove stdout/*
    COMMAND ${CMAKE_COMMAND} -E remove FS/*
    COMMAND ${VSIM} -64 -c -do 'source tcl_files/$<TARGET_PROPERTY:${NAME},TB>\\; run -a\\; exit\\;' > vsim.log
    COMMAND tail -n +1 -- ./stdout/*
    WORKING_DIRECTORY ./${SUBDIR}
    DEPENDS ${NAME}.slm.cmd ${NAME}.stim.txt
    COMMENT "Running ${NAME} in ModelSim"
    USES_TERMINAL)

  # testing targets
  add_test(NAME ${NAME}_test
    COMMAND ${UTILS_DIR}/run-and-check.sh ${VSIM} $<TARGET_PROPERTY:${NAME},TB>
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR})


  # if there are labels available for testing
  if(ARG_LABELS)
    set_tests_properties(${NAME}_test PROPERTIES LABELS "${ARG_LABELS}")
  endif()

endmacro()

add_subdirectory(helloworld)
add_subdirectory(coremark)
add_subdirectory(timer_test)
