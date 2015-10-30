set_source_files_properties(
  testLoadStore.c
  PROPERTIES COMPILE_FLAGS "-DOR10N_TESTS_PRINTF")

add_application(testLoadStore testLoadStore.c LABELS "riscv_tests")
