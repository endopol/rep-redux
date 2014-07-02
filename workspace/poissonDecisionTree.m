function fsm = poissonDecisionTree(lambda, max_depth, n_meas, n_dec, strict)
% Lambda    - average number of children
% max_depth - maximum allowed growth
% n_dec     - number of possible unique decisions
% n_dec     - number of possible unique measurements
% strict    =1 - only allow trees that have grown to max_depth
%           =0 - allow any tree

% fsm = {map_1, map_2, ...}, where 
%  map_i = [[node_i.measurement_1, node_i.decision_1, node_i.state_1]; ...]

if nargin<5
    strict = 1;
end
if nargin<3
    n_dec = max_depth*2;
    n_meas = max_depth*2;
end


while 1    
    fsm = {};
    depths = [0];
    
    max_state = 1;
    bp = 0;
    
    while bp<max_state
        bp = bp + 1;
        
        np = poissrnd(lambda);
        if depths(bp)==max_depth
            np = 0;
        end
        
        % generate new states
        while 1
            meas = unique(randi(n_meas, np, 1));
            if length(meas)==np
                break
            end
        end
        deci = randi(n_dec, np, 1);
        
        fsm = [fsm, {[meas, deci, max_state+(1:np)']}];
        depths = [depths (depths(bp)+1)*ones(1,np)];               
                
        max_state = max_state + np;        
    end
    
    if (~strict) || (max(depths)>=max_depth)
        break
    end
    
end


end

