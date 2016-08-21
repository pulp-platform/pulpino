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

if(CMAKE_C_COMPILER MATCHES "riscv(.*)")
  set(RISCV 1)
  message("NOTE: Detected RISCV compiler, using RISCV setup")

  if(${GCC_ETH})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -march=IMXpulpv2 -Wa,-march=IMXpulpv2")
    set(CMAKE_OBJDUMP_FLAGS -Mmarch=IMXpulpv2 -d)
  else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -march=RV32IM -Wa,-march=RV32IM")
    set(CMAKE_OBJDUMP_FLAGS -Mmarch=RV32IM -d)
  endif()

  if(${RVC})
    message("NOTE: Using compressed instructions")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mrvc")
  endif()


  set(LDSCRIPT      "link.riscv.ld")
  set(LDSCRIPT_BOOT "link.boot.ld" )

elseif(CMAKE_C_COMPILER MATCHES "or1k(.*)")
  set(RISCV 0)
  message("NOTE: Detected or1k compiler, using OR1K setup")

  # Clang supports a new ABI that takes care of the variadic function problem if
  # the function is implicitely defined
  if(CMAKE_C_COMPILER_ID MATCHES "(C|c?)lang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mabi=new")
  endif()

  if(CMAKE_OBJDUMP MATCHES "llvm-objdump")
    set(CMAKE_OBJDUMP_FLAGS -arch=or1kle -mattr=mac3,mul16,lv32,crtl -mcpu=pulp3 -symbolize -print-imm-hex -show-hr-enc -section-headers -disassemble -r -s)
  else()
    set(CMAKE_OBJDUMP_FLAGS -d)
  endif()

  set(LDSCRIPT      "link.or1k.ld")
  set(LDSCRIPT_BOOT "link.boot.ld")
endif()


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

if(${RISCV})
  set(crt0      "ref/crt0.riscv.S")
  set(crt0_boot "ref/crt0.boot.S")
else()
  set(crt0      "ref/crt0.or1k.S")
  set(crt0_boot "ref/crt0.or1k.S") # dummy, not functional
endif()

include_directories(libs/sys_lib/inc)
include_directories(libs/Arduino_lib/core_libs/inc)
include_directories(libs/Arduino_lib/separate_libs/inc)

set_source_files_properties(${crt0} PROPERTIES LANGUAGE C)
add_library(crt0 OBJECT ${crt0})

add_library(crt0_boot OBJECT ${crt0_boot})
set_target_properties(crt0_boot PROPERTIES COMPILE_FLAGS "-DBOOT")


################################################################################
# Other targets
################################################################################

if(${RISCV})
  set(PULP_CORE "riscv")
else()
  set(PULP_CORE "OR10N")
endif()


add_custom_target(vcompile
  COMMAND tcsh -c "env PULP_CORE=${PULP_CORE} ./vcompile/build_rtl_sim.csh"
  WORKING_DIRECTORY ${PULP_MODELSIM_DIRECTORY}
  ${USES_TERMINAL})

add_custom_target(vcompile.ps
  COMMAND tcsh -c "env PULP_CORE=${PULP_CORE} ./vcompile/vcompile_ps.csh"
  WORKING_DIRECTORY ${PULP_MODELSIM_DIRECTORY}
  ${USES_TERMINAL})

add_custom_target(vcompile.pl
  COMMAND tcsh -c "env PULP_CORE=${PULP_CORE} ./vcompile/vcompile_pl.csh"
  WORKING_DIRECTORY ${PULP_MODELSIM_DIRECTORY}
  ${USES_TERMINAL})

add_custom_target(vcompile.core
  COMMAND tcsh ./vcompile/vcompile_riscv.csh
  WORKING_DIRECTORY ${PULP_MODELSIM_DIRECTORY}
  ${USES_TERMINAL})


################################################################################

add_subdirectory(libs/string_lib)
add_subdirectory(libs/sys_lib)
add_subdirectory(libs/Arduino_lib)
add_subdirectory(libs/bench_lib)

add_subdirectory(apps)
