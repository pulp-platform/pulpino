set(libs beebs.core)
set(beebs.md5_SOURCES md5.c)

add_application(beebs.nettle-md5 "${beebs.md5_SOURCES}" LABELS "beebs" LIBS "${libs}")
