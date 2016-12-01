set(libs beebs.core)
set(beebs.rijndael_SOURCES aes.c aesxam)

add_application(beebs.rijndael "${beebs.rijndael_SOURCES}" LABELS "beebs" LIBS "${libs}")
