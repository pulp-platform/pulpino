set(libs beebs.core)
set(beebs.dijkstra_SOURCES dijkstra_small.c)

add_application(beebs.dijkstra "${beebs.dijkstra_SOURCES}"
  FLAGS "-DNO_BEEBS_MALLOC" LABELS "beebs" LIBS "${libs}")
