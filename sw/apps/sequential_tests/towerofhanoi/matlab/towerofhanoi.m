% Copyright 2017 ETH Zurich and University of Bologna.
% Copyright and related rights are licensed under the Solderpad Hardware
% License, Version 0.51 (the “License”); you may not use this file except in
% compliance with the License.  You may obtain a copy of the License at
% http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
% or agreed to in writing, software, hardware and materials distributed under
% this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
% CONDITIONS OF ANY KIND, either express or implied. See the License for the
% specific language governing permissions and limitations under the License.

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
