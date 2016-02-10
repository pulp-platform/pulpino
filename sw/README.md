# Setup

The software compilation flow is based on CMake. A version of CMake >= 2.8.0 is
required, but a version greater than 3.1.0 is recommended due to support for
ninja.

CMake uses out-of-source builds which means you will need a separate build
folder for the software, e.g. `build`

    mkdir build

Then switch to the build folder and copy the cmake template configuration
script there which resides in the `sw` folder. The name of template follows the
following naming scheme: cmake-configure.{or1k/riscv}.{gcc/llvm}.sh
Choose, copy, modify and then execute this script. It will setup the build
environment for you.

Now you are ready to start compiling software!


# Compiling

Switch to the build folder and compile the application you are interested in:

    make applicationName

This command will compile the application and generate stimuli for RTL simulation using ModelSim.


To compile the RTL using ModelSim, use

    make vcompile


# Executing

To execute an application again CMake can be used. Switch to the build folder and execute

    make applicationName.vsim

to start ModelSim in GUI mode.

To use console mode, use

    make applicationName.vsimc
