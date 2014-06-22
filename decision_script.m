close all
addpath('GraphViz')

%% Special Cases

filename = 'random';
fsm = poissonDecisionTree(1, 5);

% Special case 1 (trefoil)
% filename = 'trefoil';
% fsm = {[1,1,1; 2,2,2; 3,3,3; 4,4,1], ...    % state 1 (input, output, new_state)
%        [1,1,1; 2,2,2; 3,3,3; 4,4,1], ...    % state 2
%        [1,1,1; 2,2,2; 3,3,3; 4,4,1]};       % state 3

% Special case 2 (paper example)
% filename = 'paper';
% fsm = {[1,1,2; 2,1,3], ...    % 1
%        [1,1,4; 2,1,5], ...    % 2
%        [1,1,6; 2,1,7], ...    % 3
%        [1,1,1; 2,1,1], ...    % 4
%        [1,2,1; 2,1,1], ...    % 5
%        [1,1,1; 2,1,1], ...    % 6
%        [1,2,1; 2,1,1]};       % 7

% % Special case 3a (long parity checker)
% filename = 'parity';
% fsm = {[1, 1, 2], ...
%        [1, 2, 3], ...
%        [1, 1, 4], ...
%        [1, 2, 5], ...
%        [1, 1, 6], ...
%        [1, 2, 7], ...
%        [1, 1, 8], ...
%        [1, 2, 9], ...
%        []};

% Special case 3b (long parity checker)
% filename = 'parity2';
% fsm = {[1, 1, 2], ...
%        [1, 2, 3], ...
%        [1, 1, 4], ...
%        [1, 2, 5], ...
%        [1, 1, 6], ...
%        [1, 2, 7], ...
%        [1, 1, 8], ...
%        [1, 2, 9], ...
%        [1, 1, 9]};   
   
% Special case 4 (backtrack)
% filename = 'backtrack';
% fsm = {[1, 1, 2]; [2, 1, 3]; 
%        [3, 1, 4]; [4, 1, 5];
%        [3, 2, 6]; [2, 2, 7];
%        [1, 2, 7]};
  
% Special case 5 (split)
% filename = 'split';
% fsm = {[2, 1, 2; 3, 3, 3] ...   % 1
%        [4, 4, 4], ...           % 2
%        [5, 5, 5; 1, 2, 6] ...   % 3
%        [1, 1, 7], ...           % 4
%        [6, 6, 8], ...           % 5
%        [], ...                  % 6
%        [], ...                  % 7
%        [2, 2, 9], ...           % 8
%        []};                     % 9
   
% Special case 6 (wrap)
% filename = 'wrap';
% fsm = {[1, 1, 2; 2, 2, 3], ...
%        [1, 1, 4], ...
%        [1, 2, 5], ...
%        [1, 1, 6; 2, 2, 7], ...
%        [1, 1, 8; 2, 2, 9], ...
%        [], [], [], []};

% Special case 7a (?)
% filename = 'killer1';
% fsm = {[1, 1, 2; 2, 1, 3], ...      % 1
%        [1, 2, 4], ...               % 2
%        [1, 1, 5; 2, 1, 6], ...      % 3
%        [1, 3, 7], ...               % 4
%        [1, 10, 8], ...              % 5
%        [1, 11, 9], ...              % 6
%        [1, 4, 10], ...              % 7     
%        [], ...                      % 8
%        [], ...                      % 9
%        [1, 5, 11], ...              % 10
%        [1, 1, 12; 2, 1, 13], ...    % 11
%        [1, 10, 14], ...             % 12
%        [1, 11, 15], ...             % 13
%        [], ...                      % 14
%        []};                         % 15
       
% Special case 7b (?)
% filename = 'killer2';
% fsm = {[1, 1, 2; 2, 1, 11], ...     % 1
%        [1, 2, 3], ...               % 2
%        [1, 3, 4], ...               % 3
%        [1, 4, 5], ...               % 4
%        [1, 5, 6], ...               % 5
%        [1, 1, 7; 2, 1, 9], ...      % 6
%        [1, 10, 8], ...              % 7
%        [], ...                      % 8
%        [1, 11, 10], ...             % 9
%        [], ...                      % 10
%        [1, 1, 12; 2, 1, 14], ...    % 11
%        [1, 10, 13], ...             % 12
%        [], ...                      % 13
%        [1, 11, 15], ...             % 14
%        []};                         % 15

% filename = 'simple';
% fsm = {[1,1,2; 2,2,3], [2,1,4], [1,2,5], [], []};


% filename = 'overcomplete';
% fsm = {[1,1,2; 2,2,3], [1,1,4; 2,1,5], [1,2,6; 2,2,7], [], [], [], []};
% filename = 'overcomplete_reduce';
% fsm = {[1,1,2; 2,2,3], [1,1,2; 2,1,2], [1,2,3; 2,2,3]};


% filename = 'complete';
% fsm = {[1,1,2; 2,2,3], [1,2,4; 2,1,5], [1,2,6; 2,1,7], [], [], [], []};
% filename = 'complete_reduce';
% fsm = {[1,1,2; 2,2,2], [1,2,2; 2,1,2]};

   
%% Compute Reduced Representations
fprintf('\nInitial FSM:\n')
printFSM(fsm);

[reduced, B, A] = reduce_fsm_quick(fsm);

fprintf('Reduced FSM:\n')
printFSM(reduced)
fprintf('\n')

fprintf('# nodes in reduced FSM: %i\n', length(reduced))
fprintf('Naive lower bound: %i\n', lowerbound(reduced));

%% Standard Visualizations
figure; plotFSM(fsm)


figure; plotFSM(reduced, B)
xlim([-2, 2])


%% OPTIONAL Graphviz Visualizations
fprintf('\n')
plotFSM_graphviz(fsm, sprintf('%s_orig.dot', filename));
plotFSM_graphviz(reduced, sprintf('%s_reduced.dot', filename), B)

!sh ./drawgraph.sh