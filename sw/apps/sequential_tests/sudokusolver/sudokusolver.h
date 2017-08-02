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
