set(libs beebs.core)
set(beebs.lcdnum_SOURCES lcdnum.c)

add_application(beebs.lcdnum "${beebs.lcdnum_SOURCES}" LABELS "beebs" LIBS "${libs}")
