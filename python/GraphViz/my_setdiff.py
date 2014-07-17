
import numpy as np
import scipy
import matcompat

# if available import pylab (from matlibplot)
try:
    import matplotlib.pylab as plt
except ImportError:
    pass

def my_setdiff(A, B):

    # Local Variables: A, C, B, bits
    # Function calls: isempty, max, my_setdiff, logical, zeros
    #% MYSETDIFF Set difference of two sets of positive integers (much faster than built-in setdiff)
    #% C = my_setdiff(A,B)
    #% C = A \ B = { things in A that are not in B }
    #%  by Leon Peshkin pesha at ai.mit.edu 2004, inspired by BNT of Kevin Murphy
    return [C]