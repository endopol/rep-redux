function minelts = min_element(list, ordering)

infinities = find(list==inf, 1);
if ~isempty(infinities)
    minelts = inf;
    return
end

n = size(ordering, 1);

killer = zeros(1,n);
killer(list) = 1;

killed = ordering'*killer';
minelts = find(~killed);

minelts = intersect(minelts, list);

end

