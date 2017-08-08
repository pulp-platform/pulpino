cmake_minimum_required (VERSION 2.8)

include(CMakeParseArguments)

if (${CMAKE_VERSION} VERSION_GREATER 3.1.0)
  set(USES_TERMINAL "USES_TERMINAL")
else()
  set(USES_TERMINAL "")
endif()

# Force object file extension to be .o
set(UNIX TRUE CACHE STRING "" FORCE)

# System name
# If we set the system to Clang/GCC we get "-rdynamic"
# however we need Linux for dynamic linking stuffs.
# We should probably create a custom system name
set(CMAKE_SYSTEM_NAME "Linux-CXX")

enable_language(C CXX ASM)
project (pulp_software C)

enable_testing()

set(RISCV 1)

if(${RISCY_RV32F} AND ${USE_ZERO_RISCY})
  message(SEND_ERROR "Floating Point Extensions are not supported on zero-riscy")
endif()

# assign default architecture flags if not defined
string(COMPARE EQUAL "${GCC_MARCH}" "" GCC_MARCH_NOT_SET)

if(GCC_MARCH_NOT_SET)
   message("\nUsing default architecture flags!!\n")

   if(${USE_RISCY})
      if(${RISCY_RV32F})
         set(GCC_MARCH "IMFDXpulpv2")
      else()
         set(GCC_MARCH "IMXpulpv2")
      endif()
   else()
      if(${ZERO_RV32M})
        set(GCC_MARCH "RV32IM")
      else()
        set(GCC_MARCH "RV32I")
      endif()
   endif()
endif()


message(STATUS "GCC_MARCH= ${GCC_MARCH}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -march=${GCC_MARCH} -Wa,-march=${GCC_MARCH}")
set(CMAKE_OBJDUMP_FLAGS -Mmarch=${GCC_MARCH} -d)

if(${GCC_MARCH} MATCHES "IMFDXpulpv2")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mhard-float")
endif()

if(${RVC})
  message("NOTE: Using compressed instructions")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mrvc")
endif()

if(${ZERO_RV32E})
  if (${USE_RISCY})
    message(SEND_ERROR "RV32E can only be used with Zero-riscy")
  endif()
  message("NOTE: Using RV32E ISA")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m16r")
endif()

set(LDSCRIPT      "link.riscv.ld")
set(LDSCRIPT_BOOT "link.boot.ld" )


set(PULP_PC_ANALYZE "pulp-pc-analyze" CACHE PATH "path to pulp pc analyze binary")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra -Wall -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -fdata-sections -ffunction-sections -fdiagnostics-color=always")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${CMAKE_CURRENT_SOURCE_DIR}/ref -T${LDSCRIPT} -nostartfiles -Wl,--gc-sections")
set(BOOT_LINKER_FLAGS      "-L${CMAKE_CURRENT_SOURCE_DIR}/ref -T${LDSCRIPT_BOOT} -nostartfiles -Wl,--gc-sections")

set(CMAKE_CXX_COMPILER "${CMAKE_C_COMPILER}")
set(CMAKE_CXX_FLAGS    "${CMAKE_C_FLAGS}")

set(CMAKE_ASM_COMPILER "${CMAKE_C_COMPILER}")
set(CMAKE_ASM_FLAGS    "${CMAKE_C_FLAGS}")

set(PULP_MODELSIM_DIRECTORY "" CACHE PATH "Path to the ModelSim PULPino build")
set(VSIM "vsim" CACHE FILEPATH "Path to the vsim executable")

################################################################################

 if(${ZERO_RV32E})
   set(crt0      "ref/crt0.riscv_E.S")
   set(crt0_boot "ref/crt0.boot_E.S")
 else()
   set(crt0      "ref/crt0.riscv.S")
   set(crt0_boot "ref/crt0.boot.S")
 endif()

 include_directories(libs/sys_lib/inc)

 if(${ARDUINO_LIB})
  include_directories(libs/Arduino_lib/core_libs/inc)
  include_directories(libs/Arduino_lib/separate_libs/inc)
 endif()

 if(${RISCY_RV32F})
   include_directories(libs/math_fns_lib/inc)
 endif()

 set_source_files_properties(${crt0} PROPERTIES LANGUAGE C)
 add_library(crt0 OBJECT ${crt0})

 add_library(crt0_boot OBJECT ${crt0_boot})
 set_target_properties(crt0_boot PROPERTIES COMPILE_FLAGS "-DBOOT")


################################################################################
# Other targets
################################################################################


add_custom_target(vcompile
  COMMAND tcsh -c "./vcompile/build_rtl_sim.csh"
  WORKING_DIRECTORY ${PULP_MODELSIM_DIRECTORY}
  ${USES_TERMINAL})

add_custom_target(vcompile.ps
  COMMAND tcsh -c "env PL_NETLIST=${PL_NETLIST} ./vcompile/vcompile_ps.csh"
  WORKING_DIRECTORY ${PULP_MODELSIM_DIRECTORY}
  ${USES_TERMINAL})

add_custom_target(vcompile.pl
  COMMAND tcsh -c "env PL_NETLIST=${PL_NETLIST} ./vcompile/vcompile_pl.csh"
  WORKING_DIRECTORY ${PULP_MODELSIM_DIRECTORY}
  ${USES_TERMINAL})

add_custom_target(vcompile.core.riscy
  COMMAND tcsh ./vcompile/vcompile_riscv.csh
  WORKING_DIRECTORY ${PULP_MODELSIM_DIRECTORY}
  ${USES_TERMINAL})

add_custom_target(vcompile.core.zero
  COMMAND tcsh ./vcompile/vcompile_zero-riscy.csh
  WORKING_DIRECTORY ${PULP_MODELSIM_DIRECTORY}
  ${USES_TERMINAL})

################################################################################

add_subdirectory(libs/string_lib)
add_subdirectory(libs/sys_lib)

if(${RISCY_RV32F})
 add_subdirectory(libs/math_fns_lib)
endif()

if(${ARDUINO_LIB})
 add_subdirectory(libs/Arduino_lib)
endif()

add_subdirectory(libs/bench_lib)

set(BEEBS_LIB 0)

if(${BEEBS_LIB})
 add_subdirectory(libs/beebs_lib)
endif()

add_subdirectory(apps)
