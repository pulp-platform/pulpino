#!/bin/tcsh
source scripts/colors.sh

echo "${Green} -> Compiling Simulation Models..."
echo "${Green}Compiling component:   ${Brown} work.models ${NC}"
echo "${Green}library: work ${NC}"

echo "${Red}"

vlog -quiet +define+SPEEDSIM ../imperio/sourcecode/models/spi_flash/S25fs256s/model/s25fs256s.v     || exit 1
vlog -quiet                  ../imperio/sourcecode/models/24xx1025/24FC1025.v                       || exit 1
# vlog -quiet                  ../imperio/sourcecode/models/24xx1025/24AA1025.v                       || exit 1
# vlog -quiet                  ../imperio/sourcecode/models/24xx1025/24LC1025.v                       || exit 1
# vlog -quiet                  ../imperio/sourcecode/models/23A1024/23A1024.v                         || exit 1
# vlog -quiet                  ../imperio/sourcecode/models/mram/mr10q010/mr10q010.v                  || exit 1

echo "${Cyan} Simulation model files have been compiled Succesfully${NC}"
