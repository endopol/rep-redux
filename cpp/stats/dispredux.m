%% Display various statistics for reductions on given datasets
%   1. Poisson Tree Growth Trend - distribiution (vertical histograms) 
%     of tree sizes against the depth of the tree.
%   2. FSM Reduction Strategies - distribution of reduced tree sizes,
%     for various methods, against the original size of the tree.
%   3. FSM Reduction Strategies vs Exact - same as the above, plotted
%     against the size of the minimally reduced tree.
%   4. Timing - Elapsed time for various reduction methods, plotted
%     against the original size of the tree.

POISS = 1; PATHO = 2;

dataset = POISS;

if dataset == PATHO
    MAX_DEPTH = 100;
    D_STEP = 5;
    NSAMPLES = 100;
    DATANAME = 'patho_data';
elseif dataset == POISS
    MAX_DEPTH = 12;
    D_STEP = 2;
    NSAMPLES = 1000;
    DATANAME = 'data';
end

%% Read data

alldat = []; depth = [];
for i = D_STEP:D_STEP:MAX_DEPTH
    filename = sprintf('%s-%02d.txt',DATANAME,i);
    page = importdata(filename);
    data = page.data;

    depth = [depth; i*ones(NSAMPLES,1)];
    gapsize = max(0, size(alldat,2) - (size(data,2)));
    alldat = [alldat; [data((1:NSAMPLES),:), zeros(NSAMPLES, gapsize)]];
end

tsize = alldat(:, strmatch('N_STATES', page.colheaders));
dnsty = alldat(:, strmatch('_DENSITY', page.colheaders));

lo_bnd = alldat(:, strmatch('LO_BOUND', page.colheaders));
nexact = alldat(:, strmatch('_N_EXACT', page.colheaders));
texact = alldat(:, strmatch('_T_EXACT', page.colheaders));

ncens = alldat(:, strmatch('_N_CENSI', page.colheaders));
tcens = alldat(:, strmatch('_T_CENSI', page.colheaders));

ngree = alldat(:, strmatch('N_GREEDY', page.colheaders));
tgree = alldat(:, strmatch('T_GREEDY', page.colheaders));

njosh = alldat(:, strmatch('__N_JOSH', page.colheaders));
tjosh = alldat(:, strmatch('__T_JOSH', page.colheaders));

nalbt = alldat(:, strmatch('N_ALBRTO', page.colheaders));
talbt = alldat(:, strmatch('T_ALBRTO', page.colheaders));

%% 1. Poission-Tree Growth Trend
figure(1)
cla
[x_list, C] = vert_hist(depth, log(tsize), 'blue', 20);
min_y = min(C)-1; max_y = max(C)+1;
for i=1:length(x_list)
    x = x_list(i);
    plot([x x], [min_y, max_y], 'k:')
end
title('Poisson-Tree Growth Trend')
xlabel('Tree depth')
ylabel('log(Tree size)')
hold on

%% 2. FSM Reduction Strategies
mindim1 = min(max(tsize), max([max(ncens), max(njosh), max(nalbt)]));
figure(2)
cla
vert_hist(tsize, nalbt, 'red', tsize, njosh, 'green', tsize, ncens, 'blue')
hold on
title('FSM Reduction Strategies')
xlabel('Original size')
ylabel('Reduced fsm size')
plot((0:mindim1+1), (0:mindim1+1), 'k:')

%% 3. FSM Reduction Strategies vs Exact
mindim2 = min(max(nexact), max([max(ncens), max(njosh), max(nalbt)]));
figure(3)
cla
vert_hist(nexact, nalbt, 'red', nexact, ncens,'blue',  nexact, njosh, 'green')
hold on
title('FSM Reduction Strategies vs Exact')
xlabel('Minimum size')
ylabel('Reduced fsm size')
plot((0:mindim2+1), (0:mindim2+1), 'k:')

%% 4. Timing
figure(4)
semilogy(tsize, texact, 'k.', tsize, tcens, 'b.', tsize, tjosh, 'g.', tsize, talbt, 'r.')
% vert_hist(tsize, texact, 'black', tsize, tcens, 'blue', tsize, tjosh, 'green', tsize, talbt, 'red')
title('Timing')
xlabel('Original size')
ylabel('Computation Time (ms)')
legend('Exact', 'Censi', 'Josh', 'Alberto')