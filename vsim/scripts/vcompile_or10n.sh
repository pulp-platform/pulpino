#!/bin/tcsh
source scripts/colors.sh

setenv MSIM_LIBS_PATH `pwd`/modelsim_libs
setenv IPS_PATH `pwd`/../ips

echo ""
echo "${Green}--> Compiling or10n core... ${NC}"

source ${IPS_PATH}/scripts/vcompile_or10n.sh || exit 1

echo "${Green}--> or10n core compilation Complete! ${NC}"
echo ""
