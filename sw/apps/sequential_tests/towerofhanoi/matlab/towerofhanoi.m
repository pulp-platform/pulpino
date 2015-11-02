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
