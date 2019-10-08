#!/bin/bash -e

BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m' # No Color
BOLD='\033[1m'
STD='\033[0m'

if [ ! -e fpga ]; then
    git clone git@gitlab.greenwaves-tech.com:gwt_hw_developer_restricted/gap9_fpga.git fpga
    pushd fpga > /dev/null
    echo -e "${BOLD}${BLUE}Cloning fpga${NC}${STD}"
else
    pushd fpga > /dev/null
    git checkout master
    git pull
    echo -e "${BOLD}${BLUE}Updating fpga${NC}${STD}"
fi

ENV_COMMIT=pulpino

git checkout ${ENV_COMMIT}
echo -e "${BOLD}${BLUE}Moving to commit ${ENV_COMMIT}${NC}${STD}"

popd > /dev/null
