function z = soft_max(x, y)
    maxi = max(x, y);
    mini = min(x, y);
    z = maxi + log(1+exp(mini-maxi));
end