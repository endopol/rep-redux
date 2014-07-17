
import numpy as np
import scipy
import matcompat

# if available import pylab (from matlibplot)
try:
    import matplotlib.pylab as plt
except ImportError:
    pass

def printFSM(fsm):

    # Local Variables: i, j, mi, fsm, n, max_length
    # Function calls: max, length, fprintf, printFSM, size
    n = length(fsm)
    max_length = 0.
    for i in np.arange(1., (n)+1):
        max_length = matcompat.max(max_length, matcompat.size(fsm.cell[int(i)-1], 1.))
        
    fprintf('---')
    for i in np.arange(1., (max_length)+1):
        fprintf('----------------')
        
    fprintf('\n')
    for i in np.arange(1., (n)+1):
        fprintf('%02i:  ', i)
        mi = matcompat.size(fsm.cell[int(i)-1], 1.)
        for j in np.arange(1., (mi)+1):
            fprintf('%02i-->(%02i, %02i),  ', fsm.cell[int(i)-1,int(j)-1,0](), fsm.cell[int(i)-1,int(j)-1,1](), fsm.cell[int(i)-1,int(j)-1,2]())
            
        fprintf('\n')
        
    fprintf('\n')
    return 