% Copyright (C) 2017 ETH Zurich, University of Bologna
% All rights reserved.
% 
% This code is under development and not yet released to the public.
% Until it is released, the code is under the copyright of ETH Zurich and
% the University of Bologna, and may contain confidential and/or unpublished
% work. Any reuse/redistribution is strictly forbidden without written
% permission from ETH Zurich.
% 
% Bug fixes and contributions will eventually be released under the
% SolderPad open hardware license in the context of the PULP platform
% (http://www.pulp-platform.org), under the copyright of ETH Zurich and the
% University of Bologna.

function grid = sudokusolver()

SUDOKUSIZE = 9;

grid = zeros(SUDOKUSIZE*SUDOKUSIZE,1);

grid = [0, 9, 3, 1, 0, 5, 6, 4, 0,...
        7, 0, 0, 0, 0, 0, 0, 0, 5,...
        5, 0, 1, 2, 0, 9, 3, 0, 7,...
        2, 0, 0, 0, 0, 0, 0, 0, 3,...
        0, 3, 6, 9, 0, 7, 5, 2, 0,...
        9, 0, 0, 0, 0, 0, 0, 0, 1,...
        3, 0, 2, 4, 0, 8, 1, 0, 9,...
        6, 0, 0, 0, 0, 0, 0, 0, 4,...
        0, 4, 7, 3, 0, 2, 8, 5, 0];

    grid = solve(grid,0,0,SUDOKUSIZE);

end


function [grid,solved] = solve(grid, row, col, SUDOKUSIZE)

solved = 0;

if row==9
    solved = 1;
else
    for n=1:9
        if safe(grid, row, col, n, SUDOKUSIZE)
            t = grid(row*SUDOKUSIZE+col+1);
            grid(row*SUDOKUSIZE+col+1) = n;
            if (col == 8)
                [grid,solved] = solve(grid, row+1, 0, SUDOKUSIZE);
            else
                [grid,solved] = solve(grid, row, col+1, SUDOKUSIZE);
            end
            if solved == 0
                grid(row*SUDOKUSIZE+col+1)=t;
            else
                break;
            end
        end
    end
end
end

function bool = safe(grid, row, col, n, SUDOKUSIZE)

bool=2;

if grid(row*SUDOKUSIZE+col+1) == n
    bool = 1;
end
if bool==2
    if grid(row*SUDOKUSIZE+col+1) ~= 0
        bool = 0;
    end
end
if bool==2
    for c=0:8
        if grid(row*SUDOKUSIZE+c+1) == n
            bool = 0;
        end
    end
end
if bool==2
    for r=0:8
        if grid(r*SUDOKUSIZE+col+1) == n
            bool = 0;
        end
    end
end
if bool==2
    br = floor(row/3);
    bc = floor(col/3);
    for r=br*3:(br+1)*3-1
        for c=bc*3:(bc+1)*3-1
            if grid(r*SUDOKUSIZE+c+1) == n
                bool = 0;
            end
        end
    end
end
if bool==2
    bool=1;
end

end