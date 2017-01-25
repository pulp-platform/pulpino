set(libs beebs.core)
set(beebs.queue_SOURCES queue.c)

add_application(beebs.sglib-queue "${beebs.queue_SOURCES}" LABELS "beebs" LIBS "${libs}")
