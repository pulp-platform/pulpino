set(perfbench.fdctfst_SOURCES fdctfst.c fdctfst_test.c)
add_application(perfbench.fdctfst "${perfbench.fdctfst_SOURCES}"
  LABELS "perfbench" LIBS perfbench.core)
