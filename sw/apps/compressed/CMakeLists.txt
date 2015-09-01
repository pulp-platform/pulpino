#set_source_files_properties(compressed.S PROPERTIES LANGUAGE C)
set(SOURCES compressed.S main.c)

add_application(compressed "${SOURCES}" FLAGS "-mrvc")
