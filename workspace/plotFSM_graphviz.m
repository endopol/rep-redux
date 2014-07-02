function plotFSM_graphviz(fsm, filename, fsm_B)

n = length(fsm);

A = zeros(n);
edge_labels = cell(n,n);
node_labels = cell(1,n);
for i=1:n
    if nargin >= 3
        node_labels(i) = {sprintf('%i', fsm_B(i))};
    end
    
    table = fsm{i};
    
    if isempty(table)
        continue
    end
    
    row = table(:,3);
    if isempty(table)
        continue
    end
    
    B = unique(row);
    for j=1:length(B);
        io = find(row==B(j));
        label = '';
        for k=1:length(io)
            meas = table(io(k), 1);
            deci = table(io(k), 2);
            label = strcat(label, sprintf('%i:%s  \\n', meas, char(64+deci)));
        end
        
        A(i, B(j)) = 1;
        edge_labels{i, B(j)} = label;
    end
end

if nargin >= 3
    graph_to_dot(A, 'arc_label', edge_labels, 'filename', filename, 'node_label', node_labels);
else
    graph_to_dot(A, 'arc_label', edge_labels, 'filename', filename);
end

end

