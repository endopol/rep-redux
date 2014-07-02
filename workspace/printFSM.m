function printFSM(fsm)

n = length(fsm);
max_length = 0;
for i=1:n
    max_length = max(max_length, size(fsm{i}, 1));
end

fprintf('---')
for i=1:max_length
    fprintf('----------------')
end
fprintf('\n')
    
for i=1:n
    fprintf('%02i:  ', i)
    mi = size(fsm{i}, 1);
    for j=1:mi
        fprintf('%02i-->(%02i, %02i),  ', fsm{i}(j,1), fsm{i}(j,2), fsm{i}(j,3))
    end
    fprintf('\n')
end
fprintf('\n')
    
end

