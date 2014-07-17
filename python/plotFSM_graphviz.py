
import numpy as np
import scipy
import matcompat

# if available import pylab (from matlibplot)
try:
    import matplotlib.pylab as plt
except ImportError:
    pass

def plotFSM_graphviz(fsm, filename, fsm_B):

    # Local Variables: A, meas, B, deci, i, k, j, fsm, n, label, io, table, fsm_B, node_labels, filename, edge_labels, row
    # Function calls: char, graph_to_dot, strcat, cell, nargin, length, zeros, sprintf, plotFSM_graphviz, isempty, unique, find
    n = length(fsm)
    A = np.zeros(n)
    edge_labels = cell(n, n)
    node_labels = cell(1., n)
    for i in np.arange(1., (n)+1):
        if nargin >= 3.:
            node_labels[int(i)-1] = cellarray(np.hstack((sprintf('%i', fsm_B[int(i)-1]))))
        
        
        table = fsm.cell[int(i)-1]
        if isempty(table):
            continue
        
        
        row = table[:,2]
        if isempty(table):
            continue
        
        
        B = np.unique(row)
        for j in np.arange(1., (length(B))+1):
            io = nonzero((row == B[int(j)-1]))
            label = \'
            for k in np.arange(1., (length(io))+1):
                meas = table[int(io[int(k)-1])-1,0]
                deci = table[int(io[int(k)-1])-1,1]
                label = strcat(label, sprintf('%i:%s  \\n', meas, np.char((64.+deci))))
                
            A[int(i)-1,int(B[int(j)-1])-1] = 1.
            edge_labels.cell[int(i)-1,int(B[int(j)-1])-1] = label
            
        
    if nargin >= 3.:
        graph_to_dot(A, 'arc_label', edge_labels, 'filename', filename, 'node_label', node_labels)
    else:
        graph_to_dot(A, 'arc_label', edge_labels, 'filename', filename)
        
    
    return 