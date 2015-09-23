# vlsi soc paper compile flags:


# hack to make sure the perfbench.* libraries compile
include_directories(
  ${CMAKE_SOURCE_DIR}/libs/malloc_lib/inc
  ${CMAKE_SOURCE_DIR}/libs/string_lib/inc
  ${CMAKE_SOURCE_DIR}/libs/stdio_lib/inc
  ${CMAKE_SOURCE_DIR}/libs/bench_lib/inc
  .
)

set(PERFBENCH_CORE_SOURCES main.c)
set(PERFBENCH_EXTRAS_SOURCES crc32.c)

add_library(perfbench.core STATIC ${PERFBENCH_CORE_SOURCES})
add_library(perfbench.extras STATIC ${PERFBENCH_EXTRAS_SOURCES})



add_subdirectory(conv2d)
add_subdirectory(fdctfst)
add_subdirectory(ipm)
#add_subdirectory(matmul)
add_subdirectory(fir)
add_subdirectory(fft)
add_subdirectory(aes_cbc)
add_subdirectory(keccak)
add_subdirectory(sha)
