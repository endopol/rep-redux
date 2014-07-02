function [F, F1, F2] = tree_force(x, y, t)

k = .5;
G = 5;
eps = 0.1;
M = 10;
dropoff = 10;

n = length(t);

F1 = zeros(size(t));
F2 = zeros(size(t));
for i=2:n
   if y(i-1) == y(i) && abs(x(i-1)-x(i))<dropoff
       F1(i) = F1(i) + G/max(x(i) - x(i-1), eps)^2;
   end
   if (i<n) && (y(i+1) == y(i)) && abs(x(i+1)-x(i))<dropoff
       F1(i) = F1(i) - G/max(x(i+1) - x(i), eps)^2;
   end
   
   Fr = k*sign(x(t(i)) - x(i))*(abs(x(t(i)) - x(i))).^1;
%    Fr = sign(x(t(i)) - x(i))*min(M, k*sqrt(1 + (x(t(i)) - x(i))^2));
   
   F2(i) = F2(i) + Fr;
%    F2(t(i)) = F2(t(i)) - Fr;
end

F = F1 + F2;

end

