// Copyright 2016 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#ifndef PULPINO_H
#define PULPINO_H

#include "Vtop.h"
#include "verilated.h"
#include <fstream>
#include <vector>
#include <sstream>

class PULPino {

public:
    PULPino();
    ~PULPino();
    void preload_memories(std::string instr_file, std::string data_file);
    void set_boot_reg(unsigned int boot_address);
    int get_return_code();

    Vtop* top;
private:

    vector<string> split(string str, char delimiter);
};

#endif