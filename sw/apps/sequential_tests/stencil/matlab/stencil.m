% Copyright 2017 ETH Zurich and University of Bologna.
% Copyright and related rights are licensed under the Solderpad Hardware
% License, Version 0.51 (the “License”); you may not use this file except in
% compliance with the License.  You may obtain a copy of the License at
% http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
% or agreed to in writing, software, hardware and materials distributed under
% this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
% CONDITIONS OF ANY KIND, either express or implied. See the License for the
% specific language governing permissions and limitations under the License.

function R = stencil(N,M)

A = zeros(N*M,1);
R = zeros(N*M,1);
W = zeros(N,1);
Neighbors = zeros(4,1);
weights = zeros(4,1);

for i=0:N-1
    for j=0:M-1
        A(i*M+j+1) = i+j+1;
%         A(i*N+j+1) = 1549578963*i-j;
    end
    W(i+1) = i+2;
%     W(i+1) = 3534442427*i;
end


for c=0:N-1
    for d=0:M-1
        [Neighbors, weights] = getEntries(c,d,A,W,N,M);
        R(c*M+d+1) = A(c*M+d+1) + Neighbors(4)*weights(4)+Neighbors(1)*weights(1)+Neighbors(2)*weights(2)+Neighbors(3)*weights(3);
%         A(c*N+d+1)
%         R(c*N+d+1)
    end
end


end


function [Neighbors,weights] = getEntries(c,d,A,W,N,M)

if (c==0)
    Neighbors(1) = 0;
    Neighbors(4) = A((c+1)*M+d+1);
    weights(1) = 0;
    weights(4) = W(c+1+1);
elseif (c==N-1)
    Neighbors(1) = A((c-1)*M+d+1);
    Neighbors(4) = 0;
    weights(1) = W(c-1+1);
    weights(4) = 0;
else
    Neighbors(1) = A((c-1)*M+d+1);
    Neighbors(4) = A((c+1)*M+d+1);
    weights(1) = W(c-1+1);
    weights(4) = W(c+1+1);
end

  
  if (d==0)
    Neighbors(2) = 0;
    Neighbors(3) = A(c*M+d+1+1);
    weights(2) = 0;
    weights(3) = W(c+1);
  elseif (d==M-1)
    Neighbors(2) = A(c*M+d-1+1);
    Neighbors(3) = 0;
    weights(2) = W(c+1);
    weights(3) = 0;
  else
    Neighbors(2) = A(c*M+d-1+1);
    Neighbors(3) = A(c*M+d+1+1);
    weights(2) = W(c+1);
    weights(3) = W(c+1);
  end
end