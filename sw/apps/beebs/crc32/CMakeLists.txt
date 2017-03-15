set(libs beebs.core)
set(beebs.crc32_SOURCES crc_32.c)

add_application(beebs.crc32 "${beebs.crc32_SOURCES}" LABELS "beebs" LIBS "${libs}")
