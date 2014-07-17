
import numpy as np
import scipy
import matcompat

# if available import pylab (from matlibplot)
try:
    import matplotlib.pylab as plt
except ImportError:
    pass

def min_element(list, ordering):

    # Local Variables: minelts, ordering, infinities, killer, list, n, killed
    # Function calls: min_element, zeros, intersect, isempty, inf, find, size
    infinities = nonzero((list == np.inf), 1.)
    if not isempty(infinities):
        minelts = np.inf
        return []
    
    
    n = matcompat.size(ordering, 1.)
    killer = np.zeros(1., n)
    killer[int(list)-1] = 1.
    killed = np.dot(ordering.conj().T, killer.conj().T)
    minelts = nonzero((not killed))
    minelts = intersect(minelts, list)
    return [minelts]