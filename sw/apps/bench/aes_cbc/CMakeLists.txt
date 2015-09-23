set(perfbench.aes_cbc_SOURCES aes_cbc.c aes_cbc_test.c)
add_application(perfbench.aes_cbc "${perfbench.aes_cbc_SOURCES}"
  LABELS "perfbench" LIBS perfbench.core)
