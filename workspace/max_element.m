function maxelts = max_element(list, ordering)

if isempty(list)
    maxelts=inf;
    return
end

n = size(ordering, 1);
if islogical(list)
    if ~any(list)
        maxelts = inf;
        return
    end
    killer = list;
else      
    killer = false(1,n);
    killer(list) = true;
end

killed = ordering * killer';
maxelts = find(killer & (~killed'));

end

