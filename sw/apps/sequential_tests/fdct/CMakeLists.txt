set_source_files_properties(
  fdct.c
  PROPERTIES COMPILE_FLAGS "-DOR10N_TESTS_PRINTF")

add_application(fdct fdct.c)
