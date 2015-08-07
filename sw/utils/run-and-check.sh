#!/bin/bash

VSIM=$1
TCL_SCRIPT=$2

rm -f ./stdout/*

$VSIM -64 \
  -c \
  -do "source tcl_files/$TCL_SCRIPT; run -a; exit;"

not_ok=`cat ./stdout/* | grep "NOT OK!!!!!"`
ok=`cat ./stdout/* | grep "OOOOOOK!!!!!!"`

if [ -n "$not_ok" ]; then
  exit 1
fi

if [ -n "$ok" ]; then
  exit 0
fi

# if nothing was output that looked like a status code, just exit with error
exit 1

