set(libs beebs.core)
set(beebs.nsichneu_SOURCES nsichneu.c)

add_application(beebs.nsichneu "${beebs.nsichneu_SOURCES}" LABELS "beebs" LIBS "${libs}")
