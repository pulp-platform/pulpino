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

# Installation (Zedboard)
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

그다음 Hardware ip들을 받기 위해 다음 스크립트를 실행한다.

    ./update-ips.py
    
## HW (FPGA) Synthesis 및 petalinux 빌드
아래는 zedboard를 위한 petalinux (BOOT.BIN, uImage, devicetree.dtb),
PULPino bitstream 및 spiloader (PS->PL로 코드전송 프로그램)를 만들기 위한 과정이다.

1. Vivado 2015.1이 정상적으로 설치되어 있고, Vivado License Manager에서 xc7z020를 지원하는지 확인한다.
<img src="https://github.com/JunyeonL/pulpino/blob/master/vivado_license.JPG">

2. 빌드하려는 코어 종류에 맞게 아래와 같이 설정한다.  (설정 안할경우 RISCY 코어로 기본 선택)
  + zero-riscy : `setenv USE_ZERO_RISCY 1`  and `setenv ZERO_RV32M 1`
  + RISCY : `setenv USE_ZERO_RISCY 0`

3. fpga 폴더로 이동한다.

4. 빌드 명령어를 실행한다 (bitstream, petalinux 모두 빌드하기 때문에 많은 시간이 소요된다)

    ```
    $ make all
    ```

5. 빌드가 성공적으로 끝나면 `fpga/sw/sd_image` 폴더에 생성된 이미지를 확인한다. 정상적으로 빌드 완료 되었다면 BOOT.BIN, devicetree.dtb, rootfs.tar, uImage 등이 생성된다.

6. spiloader (petalinux 위에서 동작하는 앱) 빌드를 위해 `sw/apps/spiload`로 이동한다.

7. `make` 명령어를 실행한다. (여기서 arm-xilinx-linux-gnueabi-gcc가 없다는 에러가 발생한다면 Vivado 및 Vivado SDK의 settings64.sh를 실행했는지 다시 확인해본다. 반드시 필요)

8. 컴파일이 정상적으로 된다면 spiload 실행 파일이 생성된다.

9. Zedboard Boot image를 굽기 위한 SD카드를 준비한다.

  + 참고 : https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/18841655/Prepare+Boot+Medium
    
10. SD카드의 boot 파티션에는 BOOT.BIN, devicetree.dtb, uImage를 넣고, root 파티션에는 rootfs.tar를 압축 해제한다.

```
    $ cp BOOT.BIN /path-to-boot-partition/
    $ cp devicetree.dtb /path-to-boot-partition/
    $ cp uImage /path-to-boot-partition/

    $ tar -xvf rootfs.tar /path-to-root-partition/.
```

11. 5번 단계에서 생성한 spiload 프로그램을 SD카드 root partition의 home 폴더에 복사한다.
```
    $ cp ./sw/apps/spiload/spiload /path-to-root-partition/home/
```

12. SD카드를 Zedboard에 넣고, petalinux가 정상적으로 부팅되는지 확인한다. 만약 정상적으로 로그가 뜬다면,
   buildroot 메시지를 볼 수 있고, `username : root` 를 입력하면 된다.


## RISCV 어플리케이션 컴파일

1. 반드시 위의 HW 과정을 마무리하여야 하고, petalinux 부팅이 정상적으로 이루어진 뒤 다음 과정을 진행하길 바란다.

2. RISCV용 샘플 코드는 pulpino/sw 폴더에 위치해 있고, helloworld, gpio, freertos 등 다양한 샘플 코드를 지원한다.

3. RISCV 소스 코드 컴파일 과정에 Vivado(xilinx) 컴파일러와 충돌 문제가 있으므로, 새로운 터미널을 열어서 RISCV 소스코드 컴파일 하는 것을 추천한다. (새로운 터미널은 Vivado 및 Vivado SDK에서 지원하는 settings64.sh 스크립트가 실행되지 않은 환경 이어야 한다.)

4. pulpino/sw/ 폴더 안에 build 폴더를 만든다.
    ```
    $ cd sw
    $ mkdir build
    ```
    
5. sw폴더에 있는 cmake_configure.riscv.gcc.sh 스크립트를 build 폴더로 복사한다.
    ```
    $ cp ../cmake_configure.riscv.gcc.sh .
    ```
6. cmake_configure.riscv.gcc.sh를 열어서 아래 부분을 수정한다.
    ```
    -TARGET_C_FLAGS="-O3 -m32 -g"
    +TARGET_C_FLAGS="-O3 -march=rv32g -g"

    -GCC_MARCH="IMXpulpv2"
    
    -ARDUINO_LIB=1
    +ARDUINO_LIB=0
    ```

3. Transfer this program to the ZYNQ. We suggest using scp, but any other
   method works as well of course.

5. Now you need to compile the program you want to run on PULPino.
   Please take a look at the README in pulpino/sw directory which explains how
   applications can be compiled using cmake.
   Use this flow to compile your application. We need the spi_stim.txt file
   from the applications slm_files subfolder.

6. Transfer the spi_stim.txt file to the ZYNQ.

7. Run the spiload application on the ZYNQ like this

    ./spiload ./spi_stim.txt

   This resets PULPino, transfers the application to the memories of PULPino
   and starts it.


As an alternative, there is a cmake target for running applications on fpga
directly. Just call

    make applicationName.fpga

You need to be able to ssh into the Linux running on the ZYNQ fpga (e.g. using
public keys) and you need to setup the environment variable `$FPGA_HOSTNAME`.
Take a look at the script `./sw/utils/run-on-fpga.sh` to understand how it
works.


## stdout via printf on PULPino

When PULPino is run on the FPGA, it transfers all output via printf via UART to
the ARM host processor in the ZYNQ. To display it, either use a console program
like minicom to read directly from the serial port, or specify a timeout when
using `spiload`. `spiload` will connect to the serial port and display
everything that PULPino sends via UART until the timeout expires.

## Connected peripherals & communication with PULPino

PULPino includes a set of built-in peripherals like SPI, UART and GPIOs.
The SPI slave peripheral is connected to the SPI master of the ZYNQ, thus it is
possible to directly write to any memory address of PULPino from outside.

UART is connected to UART0 of the ZYNQ and is available under /dev/ttyPS0 in
linux.


Some of GPIO pins are connected to LEDs, switches and buttons on the ZedBoard.

Specifically the following is connected:

    PULPino GPIO pin  0: SW 0
    PULPino GPIO pin  1: SW 1
    PULPino GPIO pin  2: SW 2
    PULPino GPIO pin  3: SW 3
    PULPino GPIO pin  4: SW 4
    PULPino GPIO pin  5: SW 5
    PULPino GPIO pin  6: SW 6
    PULPino GPIO pin  7: SW 7

    PULPino GPIO pin  8: LD 0
    PULPino GPIO pin  9: LD 1
    PULPino GPIO pin 10: LD 2
    PULPino GPIO pin 11: LD 3
    PULPino GPIO pin 12: LD 4
    PULPino GPIO pin 13: LD 5
    PULPino GPIO pin 14: LD 6
    PULPino GPIO pin 15: LD 7

    PULPino GPIO pin 16: BTNC
    PULPino GPIO pin 17: BTND
    PULPino GPIO pin 18: BTNL
    PULPino GPIO pin 19: BTNR
    PULPino GPIO pin 20: BTNU




[1] http://www.wiki.xilinx.com/Prepare+Boot+Medium

