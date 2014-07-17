
import numpy as np
import scipy
import matcompat

# if available import pylab (from matlibplot)
try:
    import matplotlib.pylab as plt
except ImportError:
    pass

def make_layout(adj):

    # Local Variables: idx, level, i, l, N, tps, offset, y, x, adj
    # Function calls: toposort, poset, max, length, isempty, rem, zeros, make_layout, find, size
    #%  [x, y] = make_layout(adj)  Creates a layout from an adjacency matrix
    #%
    #% INPUT:   adj - adjacency matrix (source, sink)
    #% OUTPUT: x, y - Positions of nodes
    #%
    #% WARNING: Uses some very simple heuristics, any algorithm would do better 
    N = matcompat.size(adj, 1.)
    tps = toposort(adj)
    if not isempty(tps):
        #% is directed ?
    level = np.zeros(1., N)
    for i in tps:
        idx = nonzero(adj[:,int(i)-1])
        if not isempty(idx):
            l = matcompat.max(level[int(idx)-1])
            level[int(i)-1] = l+1.
        
        
        
    else:
        level = poset(adj, 1.).conj().T-1.
        
    
    y = (level+1.)/(matcompat.max(level)+2.)
    y = 1.-y
    x = np.zeros(matcompat.size(y))
    for i in np.arange(0., (matcompat.max(level))+1):
        idx = nonzero((level == i))
        offset = (plt.rem(i, 2.)-0.5)/10.
        x[int(idx)-1] = np.arange(1., (length(idx))+1)/(length(idx)+1.)+offset
        
    return [x, y]
def poset(adj, root):

    # Local Variables: idx, adj, N, queue, depth, r, idx2, root
    # Function calls: all, zeros, isempty, poset, find, size
    #% [depth] = poset(adj, root)   Identify a partial ordering among the nodes of a graph
    #% INPUT:   adj  -  adjacency matrix
    #%          root -  node to start with
    #% OUTPUT:  depth - Depth of the node
    #% Note     : All Nodes must be connected
    adj = adj+adj.conj().T
    N = matcompat.size(adj, 1.)
    depth = np.zeros(N, 1.)
    depth[int(root)-1] = 1.
    queue = root
    while 1.:
        if isempty(queue):
            if np.all(depth):
                break
            else:
                root = nonzero((depth == 0.))
                root = root[0]
                depth[int(root)-1] = 1.
                queue = root
                
            
        
        
        r = queue[0]
        queue[0] = np.array([])
        idx = nonzero(adj[int(r)-1,:])
        idx2 = nonzero((not depth[int(idx)-1]))
        idx = idx[int(idx2)-1]
        queue = np.array(np.hstack((queue, idx)))
        depth[int(idx)-1] = depth[int(r)-1]+1.
        
    return [depth]
def toposort(adj):

    # Local Variables: dummy, idx, seq, i, indx, N, indeg, idx2, adj, outdeg
    # Function calls: toposort, max, sum, isempty, find, size
    #% [seq] = toposort(adj)  A Topological ordering of nodes in a directed graph
    #% INPUT:  adj  -  adjacency matrix 
    #% OUTPUT: seq  -  a topological ordered sequence of nodes
    #%                 or an empty matrix if graph contains cycles
    return [seq]