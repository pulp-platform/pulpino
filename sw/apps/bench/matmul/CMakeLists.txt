set(perfbench.matmul32_SOURCES matmul32.cpp matmul32_test.c)
set(libs perfbench.core perfbench.extras)

add_application(perfbench.matmul32 "${perfbench.matmul32_SOURCES}"
  SUBDIR matmul32 LABELS "perfbench"
  LIBS "${libs}")
add_application(perfbench.matmul32_t "${perfbench.matmul32_SOURCES}"
  SUBDIR matmul32_t LABELS "perfbench"
  LIBS "${libs}" FLAGS "-DTRANSPOSE_B")


set(perfbench.matmul16_SOURCES matmul16.cpp matmul16_test.c)

add_application(perfbench.matmul16 "${perfbench.matmul16_SOURCES}"
  SUBDIR matmul16 LABELS "perfbench"
  LIBS "${libs}")
add_application(perfbench.matmul16_t "${perfbench.matmul16_SOURCES}"
  SUBDIR matmul16_t LABELS "perfbench"
  LIBS "${libs}" FLAGS "-DTRANSPOSE_B")


set(perfbench.matmul8_SOURCES matmul8.cpp matmul8_test.c)

add_application(perfbench.matmul8 "${perfbench.matmul8_SOURCES}"
  SUBDIR matmul8 LABELS "perfbench"
  LIBS "${libs}")
add_application(perfbench.matmul8_t "${perfbench.matmul8_SOURCES}"
  SUBDIR matmul8_t LABELS "perfbench"
  LIBS "${libs}" FLAGS "-DTRANSPOSE_B")

