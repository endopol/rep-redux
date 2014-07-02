function plotFSM(fsm, nodenames)

if nargin<2
    nodenames = (1:length(fsm));
end

arrow=1;
if isempty(nodenames)
    arrow=0;
end

n = length(fsm);
reduced_tree = zeros(1, n);
curves = {};
loops = cell(1,n);
for i=1:n
    mi = size(fsm{i}, 1);
    for j=1:mi
        curr = fsm{i}(j,3);
        if (curr>i)&&(reduced_tree(curr)==0)
            reduced_tree(curr) = i;
        end
    end
    
    table = fsm{i};
    if isempty(table)
        continue
    end
    
    row = table(:,3);
    B = unique(row);
    for j=1:length(B);
        io = find(row==B(j));
        label = cell(length(io),1);
        for k=1:length(io)
            meas = table(io(k), 1);
            deci = table(io(k), 2);
            label{k} = sprintf('\\color{red}%i:\\color{blue}%s', meas, char(64+deci));
        end        
        
        if i==B(j)
            loops{i} = label;
        else            
            % straight lines for tree edges
            if i==reduced_tree(B(j))
                theta = 0;
            else
                theta = pi/4;
            end
            
            curves = [curves; {i, B(j), theta, label}];
        end
    end
end

[x, y] = plotTree(reduced_tree, 0);
hold on
for i=1:size(curves,1)
    plot_arc([x(curves{i,1}), x(curves{i,2})], [y(curves{i,1}), y(curves{i,2})], curves{i,3}, curves{i,4}, arrow)
end

R = get(gca, 'xlim');
Rx = R(2)-R(1);
R = get(gca, 'ylim');
Ry = R(2)-R(1);

hold on
%title('Reduced Policy')
for i=1:n
    xc = x(i);
    yc = y(i);    

    horz_space = Rx*0.1;
    if ~isempty(loops{i})
        plot(xc+Rx*0.04, yc, 'ro', 'MarkerSize', 30)
        text(xc+horz_space, yc, loops{i}, 'BackgroundColor',[.9 .9 .9]);
    end
    
    if length(nodenames)>=i
        plot(xc, yc, 'w.', 'MarkerSize', 50);
        plot(xc, yc, 'ko', 'MarkerSize', 16);    
        text(xc-Rx*.005, yc, sprintf('%i', nodenames(i)));
    end
end
hold off

end

