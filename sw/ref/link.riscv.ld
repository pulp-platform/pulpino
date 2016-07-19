/* not needed, but we need separate linker scripts anyway */
OUTPUT_ARCH(riscv)

/* required to correctly link newlib */
GROUP( -lc -lgloss -lgcc -lsupc++ )

INCLUDE link.common.ld
