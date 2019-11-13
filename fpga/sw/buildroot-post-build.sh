#!/bin/sh -e

# Print out dependencies for and relative to the makefile in fpga/sw
if [ "x$1" = "xDEPENDENCIES" ]; then
    echo "apps/spiload/spiload"
    exit 0
fi

# Alias commands
cat >"${1}/root/.profile" <<EOF
alias ls='ls'
alias ll='ls -l'
alias la='ls -lA'
EOF

# SSH
mkdir -p "${1}/root/.ssh/"
[ -f ~/.ssh/authorized_keys ] && cp ~/.ssh/authorized_keys "${1}/root/.ssh/"

# spiload
cp -a ../apps/spiload/spiload "${1}/root/"
