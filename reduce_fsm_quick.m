function [reduced, B, A] = reduce_fsm_quick(fsm, verbose)
n = length(fsm);

if nargin<2
    verbose = 0;
end

%% Generate chart
chart = cell(n,n);
for i=1:n
    for j=1:(i-1)
        ij = combine(fsm{i}, fsm{j});
        chart{i, j} = ij;
    end
end

if verbose
    fprintf('Initial Chart:\n')
    print_chart(chart)
end

% iteratively reduce chart
iter = 1;
change = 1;
while change
    change = 0;
    
    for j=1:n
        for i=(j+1):n
            if isempty(chart{i,j})
                flag = 1;
                eqs = {};
            else
                flag = chart{i, j}{3};
                eqs = chart{i, j}{2};                       
            end
            
            % Ignore previously-killed cells
            if flag==0
                continue
            end
            
            for k=1:length(eqs)
                row = eqs{k};
                
                for ik=1:length(row)
                    for jk=1:(ik-1)
                        lookup = chart{row(ik), row(jk)};
                        if isempty(lookup)
                            flag = 1;
                            break
                        elseif lookup{3} == 0;
                            flag = 0;
                            break
                        end
                    end
                end
            end
            
           if ~flag
               chart{i, j}{3} = 0;
               change = 1;
           end            
        end
    end
    
    if verbose
        fprintf('Iteration %i:\n', iter)
        print_chart(chart);
        iter = iter + 1;
        
        if ~change
            fprintf('No change.\n\n')
        end
    end
end

% Calculate maximal cliques
A = zeros(n); % Compatibility Map
for i=1:n
    A(i,i) = 1;
    for j=1:(i-1)
        if isempty(chart{i,j})
            A(j,i) = 1;
            A(i,j) = 1;
        else
            A(j,i) = chart{i,j}{3};
            A(i,j) = chart{i,j}{3};
        end
    end
end

G = zeros(n); % FSM graph
for i=1:n
    row = fsm{i};
    mi = size(row,1);
    for j=1:mi
        G(row(j,3), i) = 1;
    end
end
G = full(graphminspantree(sparse(G)));
GG = zeros(n); % FSM ordering
for i=1:n
    GG = GG + eye(n);
    GG = GG * G;
end

cliques = {};
equiv = zeros(1,n);
unused = true(1,n);
while any(unused)
    start = max_element(unused, GG);
    start = start(1);

    clique = [];
    untried = unused;
    unused(start) = 0;
    while any(untried)
        next_try = max_element(untried, GG);
        next_try = next_try(1);
        untried(next_try) = 0;
        
        compatible = 1;
        for i=1:length(clique)
            if ~A(next_try, clique(i))
                compatible = 0;
                break
            end
        end
        if compatible
            clique = [clique, next_try];
            equiv(next_try) = start;
            unused(next_try) = 0;
        end
    end
    cliques = [cliques, {clique}];
end



% Generate reduced FSM
fprintf('Final equivalence classes: ')
[B, I, J] = unique(equiv, 'first');
for i=1:length(I)
    class = find(J==i);
    fprintf('{%i', class(1))
    for j=2:length(class)
        fprintf(', %i', class(j));
    end
    fprintf('}  ');
end
fprintf('\n\n')

for i=1:n % fix mappings
    if ~isempty(fsm{i})
        fsm{i}(:,3) = J(fsm{i}(:,3));
    end
end

reduced = {fsm{I}};

for i=1:n % combine states
    [combined, flag] = final_combine(reduced{J(equiv(i))}, fsm{i});
    if isempty(combined)
        reduced{J(equiv(i))} = {};
    elseif flag
        reduced{J(equiv(i))} = combined;
    end
end



end

function ab = combine(a, b)

eqs = {};
map = [];

flag = 1;

ab = [a; b];
if isempty(ab)
    return
end

[in, ~, J] = unique(ab(:,1));
for i=1:length(in)
    crop = (J==i);
    
    deci = unique(ab(crop,2));
    if length(deci)>1
        flag = 0;
        break;
    end
    
    new_states = unique(ab(crop, 3));
    
    map = [map; [ab(i), deci, new_states(1)]];
    eqs = [eqs, {new_states'}];        
end

ab = {map, eqs, flag};

end

function [ab, flag] = final_combine(a, b)


ab = [a; b];
flag = 1;

n = size(ab, 1);

[ins, I] = unique(ab(:, 1));
if length(ins)<n
    flag = 0;
end

ab = ab(I, :);

end

function print_chart(chart)
n = size(chart, 1);

max_neq = 0;
max_eq = 0;

for i=1:n
    for j=1:(i-1)
        curr = chart{i,j};
        if isempty(curr)
            continue
        end
        
        eqs = curr{2};
        flag = curr{3};
        if flag
            max_neq = max(max_neq, length(eqs));
            for k=1:length(eqs)
                max_eq = max(max_eq, length(eqs{k}));
            end
        end
    end
end

for i=2:n
    for j=1:(i-1)*(max_eq*3+2)-1
        fprintf('-')
    end
    fprintf('\n')
    for k=1:max_neq
        for j=1:(i-1)
            curr = chart{i,j};
            if isempty(curr)
                flag = 1;
                eqs = {};
            else
                eqs = curr{2};
                flag = curr{3};
            end
            for l=1:max_eq
                if flag
                    if (length(eqs)>=k)&&(length(eqs{k})>l)&&(length(eqs{k})>=2)
                        fprintf('%02i=', eqs{k}(l))
                    elseif (length(eqs)>=k)&&(length(eqs{k})==l)&&(length(eqs{k})>=2)
                        fprintf('%02i ', eqs{k}(l))
                    else
                        fprintf('   ');
                    end
                else
                    fprintf(' X ');
                end
            end
            fprintf('| ')
        end
        fprintf('\n')
    end
end
fprintf('\n')

end