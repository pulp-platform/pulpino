set(perfbench.keccak_SOURCES keccak.c keccak_test.c)
add_application(perfbench.keccak "${perfbench.keccak_SOURCES}"
  LABELS "perfbench" LIBS "perfbench.core")
