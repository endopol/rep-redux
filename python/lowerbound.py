
import numpy as np
import scipy
import matcompat

# if available import pylab (from matlibplot)
try:
    import matplotlib.pylab as plt
except ImportError:
    pass

def lowerbound(fsm):

    # Local Variables: B, curr, i, mapping, max_ambig, fsm, ambig, all_deci, n
    # Function calls: max, length, lowerbound, unique
    #%LOWERBOUND  lower bound on required number of states
    n = length(fsm)
    mapping = np.array([])
    for i in np.arange(1., (n)+1):
        mapping = np.array(np.vstack((np.hstack((mapping)), np.hstack((fsm.cell[int(i)-1])))))
        
    max_ambig = 0.
    B = np.unique(mapping[:,0])
    for i in np.arange(1., (length(B))+1):
        curr = mapping[:,0] == B[int(i)-1]
        all_deci = mapping[int(curr)-1,1]
        ambig = length(np.unique(all_deci))
        max_ambig = matcompat.max(max_ambig, ambig)
        
    return [max_ambig]