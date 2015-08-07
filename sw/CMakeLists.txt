cmake_minimum_required (VERSION 3.1)

include(CMakeParseArguments)

# Force object file extension to be .o
set(UNIX TRUE CACHE STRING "" FORCE)

# System name
# If we set the system to Clang/GCC we get "-rdynamic"
# however we need Linux for dynamic linking stuffs.
# We should probably create a custom system name
set(CMAKE_SYSTEM_NAME "Linux-CXX")

project (pulp_software C)

enable_testing()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra -Wall -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -fdata-sections -ffunction-sections -fdiagnostics-color=always")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T${CMAKE_CURRENT_SOURCE_DIR}/ref/link.ld -nostartfiles -Wl,--gc-sections")

set(PULP_MODELSIM_DIRECTORY "" CACHE PATH "Path to the ModelSim Pulp build")
set(VSIM "vsim" CACHE FILEPATH "Path to the vsim executable")

add_custom_target(vcompile
  COMMAND tcsh ./scripts/vcompile_all.sh
  WORKING_DIRECTORY ${PULP_MODELSIM_DIRECTORY}
  USES_TERMINAL)

include_directories(libs/sys_lib/inc)
set_source_files_properties(ref/crt0.S PROPERTIES LANGUAGE C)
add_library(crt0 OBJECT ref/crt0.S)


add_subdirectory(libs/stdio_lib)
add_subdirectory(libs/string_lib)
add_subdirectory(libs/sys_lib)
add_subdirectory(libs/bench_lib)

add_subdirectory(apps)
