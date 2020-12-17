#### This README.ld file was written by Sungyeong Jang and Junyeon Lee. If you have any question or need to modify it, please use pull request.

<br\><br\>


<img src="https://raw.githubusercontent.com/pulp-platform/pulpino/master/doc/datasheet/figures/pulpino_logo_inline1.png" width="400px" />

# Introduction
PULPino는 오픈소스로 개발되었으며 ETH Zurich에서 개발한 32-bit RISC-V Single core
기반 마이크로 컨트롤러 시스템이다. PULPino는 내부에 RISCY 또는 zero-riscy 코어를
선택하여 사용할 수 있다.

`USE_ZERO_RISCY = 0` or `USE_ZERO_RISCY = 1` <br/>

## RISCY Core
RISCY 코어는 in-order, single issue, 4 stages 파이프라인을 지원하며, IPC = 1에
가까운 성능을 제공한다. 또한 RISCY 코어는 RV32I, C, M (Integer, Compressed,
Multiplication) ISA를 지원하며, single-precision floating point ISA 사용 여부는
configure 할 수 있다. 

RISCY 코어는 RV32ICM(F) 와 별도의 Custom ISA를 추가로 지원하고 있다.
 1. Hardware Loops
 2. Post-incrementing load and store instructions
 3. Bit-manipulation instruction
 4. MAC operations
 5. Fixed point operations
 6. Packed-SIMD instruction and dot product
 
이러한 별도의 명령어는 Low-power signal processing이 필요한 어플리케이션을 위해
디자인하였다. RISCY의 privileged 명령어는 RISC-V의 1.9 버전 스펙을 기준으로 한다.
RISCV 1.9 privileged
 - `http://ieeexplore.ieee.org/abstract/document/7864441/` <br/>

## zero-riscy Core
zero-riscy는 RISCY보다 작은 코어로 In-order, Single issue, 2 stages 파이프라인을
지원하며, RV32I, C를 기본으로 지원한다. 또한 configuration을 통해 M(Multiplication),
E(reduced number of registers extension)을 사용할 수 있다. zero-riscy 코어는
low-area, low-power 환경을 위해 디자인 되었으며 RISCY와 마찬가지로 privileged 명령어는
1.9버전을 기준으로 한다.

이외 자세한 PULPino 스펙은 pulpino github을 참고하길 바란다.
- `https://github.com/pulp-platform/pulpino` <br/>

# Installation (Zedboard)
## Requirements
- ModelSim >= 10.2c
- CMake >= 3.1.0
- python2 >= 2.6
- pulp-riscv-gnu-toolchain (https://github.com/pulp-platform/pulp-riscv-gnu-toolchain)
- verilator 3.884 (Verilator 환경을 사용하는 경우에만)
- Vivado 2015.1

## Download source code
먼저 PULPino 프로젝트를 git clone 하여 다운받는다.
    ```
    $ git clone --recursive https://github.com/pulp-platform/pulpino.git
    ```

pulp toolchain은 다양한 sub repository들을 사용하고 있기 때문에 아래 명령어를 사용하는 것을 추천한다. 

   	$ git clone --recursive https://github.com/pulp-platform/pulp-riscv-gnu-toolchain

나머지는 해당 링크를 참조하되 Toolchain은 Installation (PULP) 의 명령어를 일부 수정하여 configure해야 한다.

    $ ./configure --prefix=/TOOLCHAIN_PATH --with-arch=rv32imc --enable-multilib

그다음 Pulpino 디렉토리에서 Hardware ip들을 받기 위해 다음 스크립트를 실행한다.

    ./update-ips.py
<br\>
    
## HW (FPGA) Synthesis 및 petalinux 빌드
아래는 zedboard를 위한 petalinux (BOOT.BIN, uImage, devicetree.dtb),
PULPino bitstream 및 spiloader (PS->PL로 코드전송 프로그램)를 만들기 위한 과정이다.<br/><br/>

1.리눅스 환경에 Vivado 2015.1이 설치되어 있고, Vivado License Manager에서 xc7z020를 지원하는지 확인한다.
<img src="https://github.com/JunyeonL/pulpino/blob/master/vivado_license.JPG">
<br/><br/>

2. 빌드하려는 코어 종류에 맞게 아래와 같이 설정한다.  (설정 안할경우 RISCY 코어로 기본 선택)
  - zero-riscy : `setenv USE_ZERO_RISCY 1`  and `setenv ZERO_RV32M 1`
  
  - RISCY : `setenv USE_ZERO_RISCY 0` <br/><br/>

3. fpga 폴더로 이동한 후 빌드 명령어를 실행한다. (bitstream, petalinux 모두 빌드하기 때문에 많은 시간이 소요된다)
    ```
    $ make all
    ```
    <br/>

5. 빌드가 성공적으로 끝나면 `fpga/sw/sd_image` 폴더에 생성된 이미지를 확인한다.
- 정상적으로 빌드 완료 되었다면 BOOT.BIN, devicetree.dtb, rootfs.tar, uImage 등이 생성된다. <br/><br/>


6. spiloader (petalinux 위에서 동작하는 앱) 빌드를 위해 `sw/apps/spiload`로 이동한다. <br/><br/>


7. `make` 명령어를 실행한다. (여기서 arm-xilinx-linux-gnueabi-gcc가 없다는 에러가 발생한다면 Vivado 및 Vivado SDK의 settings64.sh를 실행했는지 다시 확인해본다.) <br/><br/>


8. 컴파일이 정상적으로 된다면 `spiload` 실행 파일이 생성된다. <br/><br/>


9. Zedboard Boot image를 굽기위한 SD카드를 준비한다. (파티션)
  - 참고 : https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/18841655/Prepare+Boot+Medium <br/><br/>
    
    
10. SD카드의 boot 파티션에는 BOOT.BIN, devicetree.dtb, uImage를 넣고, root 파티션에는 rootfs.tar를 압축 해제한다.

    ```
    $ cp BOOT.BIN /path-to-boot-partition/
    $ cp devicetree.dtb /path-to-boot-partition/
    $ cp uImage /path-to-boot-partition/

    $ tar -xvf rootfs.tar /path-to-root-partition/.
    ```
<br/>

11. 5번 단계에서 생성한 spiload 프로그램을 SD카드 root partition의 home 폴더에 복사한다.
    ```
    $ cp ./sw/apps/spiload/spiload /path-to-root-partition/home/
    ```
<br/>

12. SD카드를 Zedboard에 넣고, petalinux가 정상적으로 부팅되는지 확인한다. 만약 정상적으로 로그가 뜬다면,
   buildroot 메시지를 볼 수 있고, `username : root` 를 입력하면 된다. <br/><br/><br/>


## RISCV 어플리케이션 컴파일

1. 반드시 위 HW 과정을 마무리하여야 하고, petalinux 부팅이 정상적으로 된 뒤 다음 과정을 진행해야한다. <br/><br/>


2. RISCV용 샘플 코드는 `pulpino/sw` 폴더에 위치해 있고, `helloworld` `gpio` `freertos` 등 다양한 샘플 코드를 지원한다. <br/><br/>


3. RISCV 소스 코드 컴파일은 Vivado(xilinx) 컴파일러와 충돌 문제가 있으므로, 새로운 터미널을 열어서 RISCV 소스코드 컴파일 하는 것을 추천한다.
 - 새로운 터미널은 Vivado 및 Vivado SDK에서 지원하는 settings64.sh 스크립트가 실행되지 않은 환경 이어야 한다. <br/><br/>


4. pulpino/sw/ 폴더 안에 build 폴더를 만든다. 
    ```
    $ cd sw
    $ mkdir build
    ```
    <br/>
    
5. sw폴더에 있는 cmake_configure.riscv.gcc.sh 스크립트를 build 폴더로 복사한다.
    ```
    $ cp ../cmake_configure.riscv.gcc.sh .
    ```
    <br/>
    
6. cmake_configure.riscv.gcc.sh를 열어서 아래 부분을 수정한다.
    ```
    -TARGET_C_FLAGS="-O3 -m32 -g"
    +TARGET_C_FLAGS="-O3 -march=rv32g -g"

    -GCC_MARCH="IMXpulpv2"
    
    -ARDUINO_LIB=1
    +ARDUINO_LIB=0
    ```
    <br/>

7. sw/build 에서 cmake_config.riscv.gcc.sh을 실행한다.
    ```
    $ ./cmake_configure.riscv.gcc.sh .
    ```
    <br/>

8. 여기서 문제가 발생할 수 있는데
 - riscv32-unknown 툴체인을 찾을 수 없다는 경우
   - https://github.com/pulp-platform/pulp-riscv-gnu-toolchain 툴체인을 다운받아 빌드한다.
   - 만약 이미 설치한 상태라면 riscv32-unknown-elf-gcc가 위치한 경로를 시스템 PATH에 추가한다.
   
 - riscv.ld 링커 스크립트를 찾을 수 없다는 경우 (아래 링크 참고)
   - https://github.com/pulp-platform/pulpino/issues/281
   - build 폴더에 CMakeFiles 폴더를 생성하여 해결할 수도 있다.
 
 - unrecognized command line option '-m32' 에러가 발생한 경우
   build 폴더 밖에 아래 스크립트를 생성하고, build 폴더 내에서 생성하여 m32 옵션을 제거해야 한다.
   ```
   #!/bin/bash

   # Find and replace all occurrances of '-m32' and fix rest of line.

   for file in $(find); do
       if [[ -f $file ]]; then
           [[ $(cat $file | grep m32) ]]
           if [[ $? == 0 ]]; then
               echo writing...
               echo $file
              sed 's/\-m32//g' $file > tmp && mv tmp $file
          fi
       fi
   done
   ```
 
<br/>

9. 정상적으로 cmake configure 스크립트가 실행되었다면, sw/build 폴더 안에서 `make helloworld` 명령어를 실행한다. <br/><br/>


10. `sw/build/apps/helloworld/slm_files/` 폴더에 `spi_stim.txt` 파일이 만들어졌다면 컴파일 성공.
  - 여기서 `spi_stim.txt`는 `helloworld` C 코드를 riscv toolchain을 이용하여 컴파일한 바이너리를 PULPino에서 실행하기 위해 PULPino Memory map을 반영한 파일이다.
  - 또한 Zedboard의 `PS(petalinux)`에서 `PL(PULPino)`로 spiload 프로그램을 이용하여 spi_stim.txt를 전송하기 위한 파일 규격을 맞추어 놓았다.
    - `pulpino/fpga/sw/apps/spiload/main.c` 프로그램 코드와 같이 분석하는 것을 추천 <br/><br/>


11. `spi_stim.txt` 파일을 SD카드 root 파티션의 home 폴더로 복사한다.
  - 만약 Zedboard와 ethernet 통신이 가능하다면 scp를 이용해서 복사할 수 있다.
  - Zedboard에서 아래와 같이 실행하면 파일을 당겨올 수 있다.

    ```
    $ scp <Linux 계정명>@<Linux IP address>:<파일경로>
    ```
    - 예시
    ```
    $ scp jun@192.168.0.11:/home/pulpino/sw/build/apps/helloworld/slm_files/spi_stim.txt
    ```

<br/>

12. petalinux에서 아래 명령어를 실행한다.
    ```
    $ ./spiload -t10 spi_stim.txt
    ```

    ```
    # ./spiload -t10 ./apps/helloworld_spi_stim.txt
    SR   is 00000001
    CCR0 is 04004005
    CCR2 is 00040080
    Device has been reset
    Sending block addr 00000000 with 256 entries
    Sending block addr 000003FC with 256 entries
    Sending block addr 000007F8 with 256 entries
    Sending block addr 00000BF4 with 256 entries
    Sending block addr 00000FF0 with 225 entries
    Sending block addr 00100000 with 25 entries
    Console Thread start
    Starting device
    Waiting for EOC...
    PULPino: Hello World!!!!!

    Timeout reached!
    Stopped after 10.7370672
    #
    ```

