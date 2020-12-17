<img src="https://raw.githubusercontent.com/pulp-platform/pulpino/master/doc/datasheet/figures/pulpino_logo_inline1.png" width="400px" />

# Introduction
PULPino는 오픈소스로 개발되었으며 ETH Zurich에서 개발한 32-bit RISC-V Single core
기반 마이크로 컨트롤러 시스템이다. PULPino는 내부에 RISCY 또는 zero-riscy 코어를
선택하여 사용할 수 있다. ("USE_ZERO_RISCY = 0" or "USE_ZERO_RISCY = 1")

## RISCY Core
RISCY 코어는 in-order, single issue, 4 states 파이프라인을 지원하며, IPC = 1에
가까운 성능을 제공한다. 또한 RISCY 코어는 RV32I, C, M (Integer, Compressed,
Multiplication) ISA를 지원하며, single-precision floating point ISA 사용 여부는
configure 할 수 있다. 

RISCY 코어는 RV32ICM(F) 이외 별도의 Custom ISA를 추가로 지원하고 있다.
 1. Hardware Loops
 2. Post-incrementing load and store instructions
 3. Bit-manipulation instruction
 4. MAC operations
 5. Fixed point operations
 6. Packed-SIMD instruction and dot product
 
이러한 별도의 명령어는 Low-power signal processing이 필요한 어플리케이션을 위해
디자인하였다. RISCY의 privileged 명령어는 RISC-V의 1.9 버전 스펙을 기준으로 한다.
RISCV 1.9 privileged : http://ieeexplore.ieee.org/abstract/document/7864441/.

## zero-riscy Core
zero-riscy는 RISCY보다 작은 코어로 In-order, Single issue, 2 stage 파이프라인을
지원하며, RV32I, C를 기본으로 지원한다. 또한 configuration을 통해 M(Multiplication),
E(reduced number of registers extension)을 사용할 수 있다. zero-riscy 코어는
low-area, low-power 환경을 위해 디자인 되었으며 RISCY와 마찬가지로 privileged명령어는
1.9버전을 기준으로 한다.

이외 자세한 PULPino 스펙은 pulpino github을 참고하길 바란다.
https://github.com/pulp-platform/pulpino

# Installation (Simulation 설명은 제외함)
## Requirements
- ModelSim >= 10.2c
- CMake >= 3.1.0
- python2 >= 2.6
- pulp-riscv-gnu-toolchain (https://github.com/pulp-platform/pulp-riscv-gnu-toolchain)
- verilator 3.884 (Verilator 환경을 사용하는 경우에만)
- Vivado 2015.1

## Download source code
PULPino는 다양한 sub repository들을 사용하고 있기 때문에 아래 명령어로
git clone하는 것을 추천한다.

    $ git clone --recursive https://github.com/JunyeonL/pulpino

그다음 Hardware ip들을 받기 위해 다음 명령어를 실행한다.

    ./update-ips.py

