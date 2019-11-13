#!/bin/sh -e

# Print out dependencies for and relative to the makefile in fpga/sw
if [ "x$1" = "xDEPENDENCIES" ]; then
    echo ""
    exit 0
fi

# SSH
mkdir -p "${1}/root/.ssh/"
[ -f ~/.ssh/authorized_keys ] && cp ~/.ssh/authorized_keys "${1}/root/.ssh/"

