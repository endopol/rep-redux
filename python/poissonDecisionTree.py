
import numpy as np
import scipy
import matcompat

# if available import pylab (from matlibplot)
try:
    import matplotlib.pylab as plt
except ImportError:
    pass

def poissonDecisionTree(lambda, max_depth, n_meas, n_dec, strict):

    # Local Variables: meas, deci, fsm, n_meas, depths, strict, bp, max_state, np, n_dec, max_depth, lambda
    # Function calls: randi, max, poissrnd, nargin, length, ones, poissonDecisionTree, unique
    #% Lambda    - average number of children
    #% max_depth - maximum allowed growth
    #% n_dec     - number of possible unique decisions
    #% n_dec     - number of possible unique measurements
    #% strict    =1 - only allow trees that have grown to max_depth
    #%           =0 - allow any tree
    #% fsm = {map_1, map_2, ...}, where 
    #%  map_i = [[node_i.measurement_1, node_i.decision_1, node_i.state_1]; ...]
    if nargin<5.:
        strict = 1.
    
    
    if nargin<3.:
        n_dec = max_depth*2.
        n_meas = max_depth*2.
    
    
    while 1.:
        fsm = cellarray([])
        depths = np.array(np.hstack((0.)))
        max_state = 1.
        bp = 0.
        while bp<max_state:
            bp = bp+1.
            np = poissrnd(lambda)
            if depths[int(bp)-1] == max_depth:
                np = 0.
            
            
            #% generate new states
            while 1.:
                meas = np.unique(randi(n_meas, np, 1.))
                if length(meas) == np:
                    break
                
                
                
            deci = randi(n_dec, np, 1.)
            fsm = np.array(np.hstack((fsm, cellarray(np.hstack((np.array(np.hstack((meas, deci, max_state+np.arange(1., (np)+1).conj().T)))))))))
            depths = np.array(np.hstack((depths, np.dot(depths[int(bp)-1]+1., np.ones(1., np)))))
            max_state = max_state+np
            
        if not strict or matcompat.max(depths) >= max_depth:
            break
        
        
        
    return [fsm]