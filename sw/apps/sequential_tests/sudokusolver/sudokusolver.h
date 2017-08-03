// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

//define sudoku size
#define SUDOKUSIZE 9

// result of sudoku
const int SUDOKUSOLVED[SUDOKUSIZE*SUDOKUSIZE] = {8, 9, 3, 1, 7, 5, 6, 4, 2, 
						 7, 2, 4, 8, 3, 6, 9, 1, 5, 
						 5, 6, 1, 2, 4, 9, 3, 8, 7, 
						 2, 1, 5, 6, 8, 4, 7, 9, 3, 
						 4, 3, 6, 9, 1, 7, 5, 2, 8, 
						 9, 7, 8, 5, 2, 3, 4, 6, 1, 
						 3, 5, 2, 4, 6, 8, 1, 7, 9, 
						 6, 8, 9, 7, 5, 1, 2, 3, 4, 
						 1, 4, 7, 3, 9, 2, 8, 5, 6};
