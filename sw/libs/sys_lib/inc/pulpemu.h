/* emulator-specific stuff */
unsigned int *PULPEMU_MEM = 0x1c007800;
#define pulpemu_store(a,b) PULPEMU_MEM[((get_core_id()<<9)+a)] = (b)
#define pulpemu_print(a)   qprintf("%08X\n", PULPEMU_MEM[((get_core_id()<<9)+a)], 0, 0, 0)

