set(libs beebs.core)
set(beebs.st_SOURCES st.c)

add_application(beebs.st "${beebs.st_SOURCES}" LABELS "beebs" LIBS "${libs}")