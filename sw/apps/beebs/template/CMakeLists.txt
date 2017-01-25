set(libs beebs.core)
set(beebs.template_SOURCES template.c)

add_application(beebs.template "${beebs.template_SOURCES}" LABELS "beebs" LIBS "${libs}")