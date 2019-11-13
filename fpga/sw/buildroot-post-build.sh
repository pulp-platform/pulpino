#!/bin/sh -e

# Print out dependencies for and relative to the makefile in fpga/sw
if [ "x$1" = "xDEPENDENCIES" ]; then
    echo ""
    exit 0
fi

