
import numpy as np
import scipy
import matcompat

# if available import pylab (from matlibplot)
try:
    import matplotlib.pylab as plt
except ImportError:
    pass

def graph_to_dot(adj, varargin):

    # Local Variables: directed, node_label, node2, node, labeltext, arcs, i, leftright, edgeformat, arctxt, height, width, Nnds, labeltxt, fid, varargin, node1, filename, adj, arc_label
    # Function calls: graph_to_dot, fclose, strcat, nargin, length, isempty, fprintf, fopen, find
    #% graph_to_dot(adj, VARARGIN)  Creates a GraphViz (AT&T) format file representing 
    #%                     a graph given by an adjacency matrix.
    #%  Optional arguments should be passed as name/value pairs [default]
    #%
    #%   'filename'  -  if omitted, writes to 'tmp.dot'
    #%  'arc_label'  -  arc_label{i,j} is a string attached to the i-j arc [""]
    #% 'node_label'  -  node_label{i} is a string attached to the node i ["i"]
    #%  'width'      -  width in inches [10]
    #%  'height'     -  height in inches [10]
    #%  'leftright'  -  1 means layout left-to-right, 0 means top-to-bottom [0]
    #%  'directed'   -  1 means use directed arcs, 0 means undirected [1]
    #%
    #% For details on dotty, See http://www.research.att.com/sw/tools/graphviz
    #%
    #% by Dr. Leon Peshkin, Jan 2004      inspired by Kevin Murphy's  BNT
    #%    pesha @ ai.mit.edu /~pesha
    node_label = np.array([])
    arc_label = np.array([])
    #% set default args
    width = 10.
    height = 10.
    leftright = 0.
    directed = 1.
    filename = 'tmp.dot'
    for i in np.arange(1., (nargin-1.)+(2.), 2.):
        #% get optional args
        
    fid = fopen(filename, 'w')
    if directed:
        fprintf(fid, 'digraph G {\n')
        arctxt = '->'
        if isempty(arc_label):
            labeltxt = \'
        else:
            labeltxt = '[label="%s", color="red", fontname="NimbusMonL-Regu", labeljust="r", fontsize=10]'
            
        
    else:
        fprintf(fid, 'graph G {\n')
        arctxt = '--'
        if isempty(arc_label):
            labeltxt = '[dir=none]'
        else:
            labeltext = '[label="%s",dir=none]'
            
        
        
    
    fprintf(fid, 'center = 1;\n')
    fprintf(fid, 'size=\"%d,%d\";\n', width, height)
    if leftright:
        fprintf(fid, 'rankdir=LR;\n')
    
    
    Nnds = length(adj)
    for node in np.arange(1., (Nnds)+1):
        #% process NODEs 
        
    edgeformat = strcat(np.array(np.hstack(('%d ', arctxt, ' %d ', labeltxt, ';\n'))))
    for node1 in np.arange(1., (Nnds)+1):
        #% process ARCs
        
    fprintf(fid, '}')
    fclose(fid)
    return 