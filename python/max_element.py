
import numpy as np
import scipy
import matcompat

# if available import pylab (from matlibplot)
try:
    import matplotlib.pylab as plt
except ImportError:
    pass

def max_element(list, ordering):

    # Local Variables: ordering, killer, list, killed, n, maxelts
    # Function calls: false, islogical, max_element, find, isempty, inf, true, any, size
    if isempty(list):
        maxelts = np.inf
        return []
    
    
    n = matcompat.size(ordering, 1.)
    if islogical(list):
        if not np.any(list):
            maxelts = np.inf
            return []
        
        
        killer = list
    else:
        killer = false(1., n)
        killer[int(list)-1] = true
        
    
    killed = np.dot(ordering, killer.conj().T)
    maxelts = nonzero(np.logical_and(killer, not killed.conj().T))
    return [maxelts]