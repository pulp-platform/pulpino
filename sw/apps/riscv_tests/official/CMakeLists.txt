set_source_files_properties(tests.S PROPERTIES LANGUAGE C)

set(SOURCES test_print.c tests.S)
add_application(rv_official    "${SOURCES}" LABELS riscv_tests)
#add_spi_regression(rv_official "${SOURCES}")
