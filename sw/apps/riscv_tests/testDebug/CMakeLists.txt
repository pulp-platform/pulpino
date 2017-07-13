if (${GCC_MARCH} MATCHES "[pulp]+")
  set(RISCV_FLAGS "-DUSE_MUL -DPULP_EXT")
elseif(${GCC_MARCH} MATCHES "RV32IM")
  set(RISCV_FLAGS "-DUSE_MUL")
else()
  set(RISCV_FLAGS "")
endif()

add_application(testDebug main.c TB run_debug.tcl FLAGS ${RISCV_FLAGS} LABELS "riscv_tests")
