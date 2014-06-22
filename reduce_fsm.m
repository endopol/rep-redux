function reduced = reduce_fsm(fsm)
n = length(fsm);

%% Generate chart
chart = cell(n,n);
for i=1:n
    for j=1:(i-1)
        ij = combine(fsm{i}, fsm{j});
        chart{i, j} = ij;
    end
end

fprintf('Initial Chart:\n')
print_chart(chart)

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
                        
                        if lookup{3} == 0;
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
    
    fprintf('Iteration %i:\n', iter)
    print_chart(chart);
    iter = iter + 1;
    
    if ~change
        fprintf('No change.\n\n')
    end
end

% Calculate maximal cliques
A = zeros(n);
for i=1:n
    for j=1:(i-1)
        if isempty(chart{i,j})
            A(j,i) = 1;
        else
            A(j,i) = chart{i,j}{3};
        end
    end
end
maxc = maximalCliques(A);
fprintf('Maximal Cliques: ')
for i=1:length(maxc)
    fprintf('{%i', maxc{i}(1))
    for j=2:length(maxc{i})
        fprintf(', %i', maxc{i}(j))        
    end
    fprintf('}  ');
end
fprintf('\n\n')


G = zeros(n);
for i=1:n
    row = fsm{i};
    mi = size(row,1);
    for j=1:mi
        G(row(j,3), i) = 1;
    end
end
G = full(graphminspantree(sparse(G)));
GG = zeros(n);
for i=1:n
    GG = GG + eye(n);
    GG = GG * G;
end

% initialize unused (get rid of singletons)
unused = zeros(1,n);
for i=1:length(maxc)
    for j=1:length(maxc{i})
        unused(maxc{i}(j)) = 1;
    end
end

equiv = zeros(1,n);
while any(unused)
    
    % Find first vacancy
    max_vacancies = cell(size(maxc));
    for i=1:length(maxc)
        if isempty(maxc{i})
            continue
        end
        
        vacancies = unused;
        vacancies(maxc{i}) = 0;
        
        max_vacancies(i) = {max_element(find(vacancies), GG)};
    end
    
    maxlist = [];
    for i=1:length(maxc)
        maxlist = [maxlist, max_vacancies{i}];
    end
    minelts = min_element(maxlist, GG);
    
    next_clique = 0;
    for i=1:length(maxc)
        if ~isempty(intersect(minelts, max_vacancies{i}))
            next_clique = i;
            break
        end
    end
    fprintf('Next clique: %i\n', next_clique)
    
    if next_clique==0
        break
    end
    
    curr = maxc{next_clique};
    
    equiv(curr) = curr(1);
    
    unused(curr) = 0;
    for i=1:length(maxc)
        maxc{i} = intersect(maxc{i}, find(unused));
    end        
end
unused = find(~equiv);
equiv(unused) = unused;

% Generate reduced FSM
fprintf('Final equivalence classes: ')
[~, I, J] = unique(equiv, 'first');
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
    combined = combine(reduced{J(equiv(i))}, fsm{i});
    reduced{J(equiv(i))} = combined{1};
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