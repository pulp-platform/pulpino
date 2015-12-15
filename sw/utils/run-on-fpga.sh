#!/bin/bash

NAME=$1

scp slm_files/spi_stim.txt $FPGA_HOSTNAME:/root/$NAME.spi
ssh -t $FPGA_HOSTNAME /root/spiload --timeout=60 /root/$NAME.spi

exit 0
