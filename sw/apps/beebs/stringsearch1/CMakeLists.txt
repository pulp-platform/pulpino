set(libs beebs.core)
set(beebs.stringsearch1_SOURCES fast.fwd.inc.c fast.rev.d12.c stringsearch1.c)

add_application(beebs.stringsearch1 "${beebs.stringsearch1_SOURCES}" LABELS "beebs" LIBS "${libs}")