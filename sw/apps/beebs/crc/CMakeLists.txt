set(libs beebs.core)
set(beebs.crc_SOURCES crc.c)

add_application(beebs.crc "${beebs.crc_SOURCES}" LABELS "beebs" LIBS "${libs}")
