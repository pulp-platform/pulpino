set(libs beebs.core)
set(beebs.levenshtein_SOURCES levenshtein.c)

add_application(beebs.levenshtein "${beebs.levenshtein_SOURCES}"
  LABELS "beebs" LIBS "${libs}")
