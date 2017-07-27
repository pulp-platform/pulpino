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

function [A,B,C] = towerofhanoi(N)

A = zeros(N,1);
B = zeros(N,1);
C = zeros(N,1);
n = N;

for i=1:n
    A(i) = i;
end

[A,B,C] = ToH(n,A,B,C);

end


function [A,B,C] = ToH(n,A,B,C)
 if n==1
   B(n) = A(n);
   A(n)=0;
 else
   [A,C,B] = ToH(n-1,A,C,B);
   B(n) = A(n);
   A(n) = 0;
   [C,B,A] = ToH(n-1,C,B,A);
 end
 
end
