#!/bin/bash
source /opt/openrisc/setup.sh
# boot/reboot board
ssh pulpemu@micrel191 "cd workspace; make copy-jenkins"
ssh pulpemu@micrel191 pulpemu_off
sleep 10
ssh pulpemu@micrel191 pulpemu_on
sleep 1
ssh pulpemu@micrel191 "pulpemu_boot; pulpemu_bitload"
sleep 10
ssh pulpemu@micrel191 "cd mem_dev; make push;"
# PULP basic tests
cd pulp_tests
cd testBarrier;        echo -e "\n***testBarrier***";        make FREQ=50 testBarrier.run     2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null ; cd ..
cd testContentionTCDM; echo -e "\n***testContentionTCDM***"; make FREQ=50 contentiontcdm.run  2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null ; cd ..
cd testCVP;            echo -e "\n***testCVP***";            make FREQ=50 testCVP.run         2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null ; cd ..
cd testEvents;         echo -e "\n***testEvents***";         make FREQ=50 testEvents.run      2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null ; cd ..
cd testGPIO;           echo -e "\n***testGPIO***";           make FREQ=50 testGPIO.run        2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null ; cd ..
cd testID;             echo -e "\n***testID***";             make FREQ=50 testID.run          2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null ; cd .. # no eoc
cd testL2;             echo -e "\n***testL2***";             make FREQ=50 testL2.run          2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null ; cd ..
cd testMCHAN;          echo -e "\n***testMCHAN***";          make FREQ=50 testMCHAN_basic.run 2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null ; cd ..
cd testMMU;            echo -e "\n***testMMU***";            make FREQ=50 testMMU.run         2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null ; cd ..
cd testmult;           echo -e "\n***testmult***";           make FREQ=50 testmult.run        2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null ; cd ..
cd testPipeStage;      echo -e "\n***testPipeStage***";      make FREQ=50 testPipeStage.run   2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null ; cd ..
cd testSPIM;           echo -e "\n***testSPIM***";           make FREQ=50 testSPIM.run        2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null ; cd ..
cd testTCDM;           echo -e "\n***testTCDM***";           make FREQ=50 testTCDM.run        2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null ; cd ..
cd testTimer;          echo -e "\n***testTimer***";          make FREQ=50 testTimer.run       2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null ; cd ..
cd testUART;           echo -e "\n***testUART***";           make FREQ=50 testUART.run        2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null ; cd ..
cd ..
# Sequential tests
cd sequential_tests
cd blowfish;      echo -e "\n***blowfish***";      make FREQ=50 bench_lib bf.run  2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd ..
cd bubblesort;    echo -e "\n***bubblesort***";    make FREQ=50 bubblesort.run    2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd ..
cd crc32;         echo -e "\n***crc32***";         make FREQ=50 crc_32.run        2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd ..
cd fdct;          echo -e "\n***fdct***";          make FREQ=50 fdct.run          2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd ..
cd fibonacci;     echo -e "\n***fibonacci***";     make FREQ=50 fibonacci.run     2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd ..
cd helloworld;    echo -e "\n***helloworld***";    make FREQ=50 helloworld.run    2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd ..
cd matrixAdd;     echo -e "\n***matrixAdd***";     make FREQ=50 matrixAdd.run     2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd ..
cd matrixMul;     echo -e "\n***matrixMul***";     make FREQ=50 matrixMul.run     2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd ..
cd matrixMulMMU;  echo -e "\n***matrixMulMMU***";  make FREQ=50 matrixMulMMU.run  2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd ..
cd stencil;       echo -e "\n***stencil***";       make FREQ=50 stencil.run       2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd ..
cd sudokusolver;  echo -e "\n***sudokusolver***";  make FREQ=50 sudokusolver.run  2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd ..
cd towerofhanoi;  echo -e "\n***towerofhanoi***";  make FREQ=50 towerofhanoi.run  2> /dev/null | perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd ..
cd ..
# Parallel tests
cd parallel_tests
cd conv16;       echo -e "\n***conv16***";       make conv16.run        2> /dev/null| perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd ..
cd DCT;          echo -e "\n***DCT***";          make DCT.run           2> /dev/null| perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd .. # no eoc
cd parMatrixMul; echo -e "\n***parMatrixMul***"; make parMatrixMul.run  2> /dev/null| perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd ..
cd SOR;          echo -e "\n***SOR***";          make SOR.run           2> /dev/null| perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd ..
cd Sparse;       echo -e "\n***Sparse***";       make Sparse.run        2> /dev/null| perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd ..
cd LU;           echo -e "\n***LU***";           make LU.run            2> /dev/null| perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd ..
cd FFT;          echo -e "\n***FFT***";          make fft_lib main.run  2> /dev/null| perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd .. # no eoc
cd ..
# OpenMP tests
cd gomp_tests
cd EPCCbench; echo -e "\n***EPCCbench***";  make syncbench.run  2> /dev/null| perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd ..
cd nrtest;    echo -e "\n***nrtest***";     make test.run       2> /dev/null| perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd ..
cd quick;     echo -e "\n***quick***";      make test.run       2> /dev/null| perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd ..
cd Dijkstra;  echo -e "\n***Dijkstra***";   make Dijkstra.run   2> /dev/null| perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd ..
cd FFT;       echo -e "\n***FFT***";        make fft.run        2> /dev/null| perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd ..
cd Sparse;    echo -e "\n***Sparse***";     make Sparse.run     2> /dev/null| perl -i -ne 'print if /^\[\W/' 2> /dev/null; cd ..
# shutdown board
ssh pulpemu@micrel191 pulpemu_off
