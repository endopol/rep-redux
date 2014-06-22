function fsm = tree_to_fsm(t,y,d)
%TREE_TO_FSM Converts decision tree to FSM
% t = [0, node_1.parent.index, node_2.parent.index ... ]
% y = [0, node_1.measurement, node_2.measurement, ...]
% d = [0, node_1.decision, node_2.decision, ...]

% output is of the form
% fsm = {map_1, map_2, ...}, where 
%  map_i = {(node_i.measurement_1, node_i.decision_1, node_i.state_1), ...}

n = length(t);

fsm = cell(1, n);
for i=1:n
    children_i = find(tree==i);
    m_i = length(children_i);
    
    fsm{i} = cell(1, m_i);
    for j=1:m_i
        curr = children_i(j);
        fsm{i}{j} = [curr, y(curr), d(curr)];
    end
end

end

