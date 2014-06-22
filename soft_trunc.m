function y = soft_trunc(x, M)
    y = -sign(x).*soft_max(-abs(x), -M);
end