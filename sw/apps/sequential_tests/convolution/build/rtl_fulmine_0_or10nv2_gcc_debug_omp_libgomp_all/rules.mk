PULP_CHIP_VERSION=0
PULP_CHIP=fulmine
PULP_CORE=or10n5
EU_VERSION=1
HOST_CFLAGS+= -I/scratch/gautschi/PULP/pulp_pipeline/pkg/sdk/dev/install/include
PULP_DEFS+= -DL2_SIZE=196608 -DL1_SIZE=65536 -DSCM_SIZE=8192 -DNB_PE=4 -DSTACK_SIZE=4096 -DNB_CLUSTER=1 -DPULP4 -D__PULP4__ -DPULP_CHIP=CHIP_FULMINE -DPULP_ARCHI=ARCHI_FULMINE -DPULP_CHIP_VERSION=0 -DPULP_CORE=CORE_OR1K_V5
PULP_INC_PATHS+= -I/scratch/gautschi/PULP/pulp_pipeline/pkg/sdk/dev/install/include/ompBare -I/scratch/gautschi/PULP/pulp_pipeline/pkg/sdk/dev/install/include
PULP_CFLAGS+= -DPULP -D__PULP__ -Wextra -Wall -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -fdata-sections -ffunction-sections -D__DEBUG__=1 -DDEBUG=1 -DCHECKINGS=1 -DOR10N5 -D__OR10N5__ -DOR1K -D__OR1K__ -D__RTL__ $(PULP_DEFS) $(PULP_INC_PATHS)
PULP_LDFLAGS+= -Wl,--section-start -Wl,.heapscm=0x10010000 -Wl,--defsym=heapscmLoad=0x10010000 -Wl,--section-start -Wl,.heapsram=0x700 -T/scratch/gautschi/PULP/pulp_pipeline/pkg/sdk/dev/install/src/link-bare-fulmine.or10n.ld -nostartfiles -nostdlib -Wl,--gc-sections -L/scratch/gautschi/PULP/pulp_pipeline/pkg/sdk/dev/install/or10nv2/lib -lbench-fulmine_0-gcc-debug -lmalloc-fulmine_0-gcc-debug -lgomp-fulmine_0-gcc-debug -lstdio-fulmine_0-gcc-debug -lstring-fulmine_0-gcc-debug -lsys-fulmine_0-gcc-debug -lmalloc-fulmine_0-gcc-debug -Wl,--gc-sections -lc -lcompilerrt-fulmine_0-gcc-debug
PULP_OMP_LDFLAGS+=
PULP_OBJDUMP = /scratch/gautschi/PULP/pulp_pipeline/pkg/or10nv2_gcc/2.0.11/bin/or1kle-elf-objdump
PULP_LD = /scratch/gautschi/PULP/pulp_pipeline/pkg/or10nv2_gcc/2.0.11/bin/or1kle-elf-gcc
PULP_CC = /scratch/gautschi/PULP/pulp_pipeline/pkg/or10nv2_gcc/2.0.11/bin/or1kle-elf-gcc -D__GCC__ -fno-jump-tables
PULP_AR = /scratch/gautschi/PULP/pulp_pipeline/pkg/or10nv2_gcc/2.0.11/bin/or1kle-elf-ar
HOST_LD = /scratch/gautschi/PULP/pulp_pipeline/pkg/or10nv2_gcc/2.0.11/bin/or1kle-elf-gcc
HOST_CC = or1kle-elf-gcc -D__GCC__
HOST_AR = /scratch/gautschi/PULP/pulp_pipeline/pkg/or10nv2_gcc/2.0.11/bin/or1kle-elf-ar
PULP_OMP_CC = /scratch/gautschi/PULP/pulp_pipeline/pkg/or10nv2_gcc/2.0.11/bin/or1kle-elf-gcc -D__GCC__ -fno-jump-tables -fopenmp
PULP_OCL_KERNEL_CFLAGS += 
PULP_OCL_KERNEL_TARGET = pulp3-or1k-bench-gcc
PULP_OMP_KERNEL_LDFLAGS +=  -nostdlib -shared -fPIC -lm
PULP_OMP_KERNEL_CFLAGS +=  -fPIC -I/scratch/gautschi/PULP/pulp_pipeline/pkg/sdk/dev/install
pulpRunOpt +=  --load=preload --binary=PP --pulp-archi=fulmine
APP_BUILD_DIR += /scratch/gautschi/PULP/pulp_pipeline/tests/tvlsi_benchmarks/convolution/build/rtl_fulmine_0_or10nv2_gcc_debug_omp_libgomp_all

scmSize=8192
nbCluster=1
systemConfig=platform=rtl:pulpChip=fulmine:pulpChipVersion=0:pulpCoreArchi=or10nv2:pulpCompiler=gcc:pulpRtVersion=debug:pulpRt=omp:pulpOmpVersion=libgomp
pulpLibs=['compilerrt']
pulpCompiler=gcc
pulpFpu=False
platform=rtl
romSize=8192
nbPe=4
pulpCoreArchi=or10nv2
pulpBinary=PP
stackSize=4096
buildDir=/scratch/gautschi/PULP/pulp_pipeline/tests/tvlsi_benchmarks/convolution/build
pulpCore=or10n5
pulpFullLibs=[]
pulpLibPaths=[]
libStringType=
pulpOmpVersion=libgomp
l2Size=196608
pulpRt=omp
sdkHome=/scratch/gautschi/PULP/pulp_pipeline/pkg/sdk/dev
or1kLlvmToolchain=/scratch/gautschi/PULP/pulp_pipeline/pkg/or1k_llvm/v1.8.6b
pulpChip=fulmine
hostCompiler=gcc
pulpDiv=False
buildDirExt=_all
l1Size=65536
pulpChipVersion=0
appBuildDir=/scratch/gautschi/PULP/pulp_pipeline/tests/tvlsi_benchmarks/convolution/build/rtl_fulmine_0_or10nv2_gcc_debug_omp_libgomp_all
pulpArchi=fulmine
pulpRtVersion=debug
pulpSharedFpu=False
MAKE_GEN_STATUS=success
