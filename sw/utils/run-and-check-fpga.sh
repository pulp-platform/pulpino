#!/bin/bash

#suffix=_spi_reg
NAME=$1
#NAME=${NAME%$suffix}
synt_log=$2
full_log=$3

function test {
  echo "$1"
  scp $1.bin pulpemu@137.204.213.191:tmp/$1.bin
  ssh pulpemu@137.204.213.191 sshpass -p root scp tmp/$1.bin root@192.168.1.10:
  ssh pulpemu@137.204.213.191 rm -rf tmp/*
  ssh pulpemu@137.204.213.191 sshpass -p root ssh root@192.168.1.10 ./run_test $1.bin 10
  stat=$?
  return $stat
}

i=0
err=0
t=${NAME}

test $t
stat=$?
if [ ${stat} -ne 0 ]; then
  if [ ${stat} -eq 7 ]; then
    echo "[$t] did not complete execution."
    exit 7
  fi
  if [ ${stat} -ne 7 ]; then
    echo "[$t] did not finish correctly, error code = ${stat}."
    exit ${stat}
  fi
fi
echo "[$t] completed execution succesfully."
exit 0

