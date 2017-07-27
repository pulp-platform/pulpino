/* Copyright (C) 2017 ETH Zurich, University of Bologna
 * All rights reserved.
 *
 * This code is under development and not yet released to the public.
 * Until it is released, the code is under the copyright of ETH Zurich and
 * the University of Bologna, and may contain confidential and/or unpublished
 * work. Any reuse/redistribution is strictly forbidden without written
 * permission from ETH Zurich.
 *
 * Bug fixes and contributions will eventually be released under the
 * SolderPad open hardware license in the context of the PULP platform
 * (http://www.pulp-platform.org), under the copyright of ETH Zurich and the
 * University of Bologna.
 */


#ifndef PULPINO_H
#define PULPINO_H

#include "Vtb_verilator.h"
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

    Vtb_verilator* top;
private:

    vector<string> split(string str, char delimiter);
};

#endif