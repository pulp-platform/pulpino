set(perfbench.conv2d_SOURCES conv2d.c conv2d_test.c)
add_application(perfbench.conv2d "${perfbench.conv2d_SOURCES}"
  LABELS "perfbench" LIBS perfbench.core)
