set(perfbench.sha_SOURCES sha.c sha_test.c)
add_application(perfbench.sha "${perfbench.sha_SOURCES}"
  LABELS "perfbench" LIBS "perfbench.core")
