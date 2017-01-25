set(libs beebs.core)
set(beebs.picojpeg_SOURCES picojpeg.c picojpeg_test.c)

add_application(beebs.picojpeg "${beebs.picojpeg_SOURCES}"
  LABELS "beebs" LIBS "${libs}")
