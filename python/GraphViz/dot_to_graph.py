
import numpy as np
import scipy
import matcompat

# if available import pylab (from matlibplot)
try:
    import matplotlib.pylab as plt
except ImportError:
    pass

def dot_to_graph(filename):

    # Local Variables: Ddash_pos, Nlns, labels, Nvrt, Lndx, Adj, arrow_pos, line_ndx, lst_node, filename, edge_id, dot_lines, node, lbl_pos, Lnode, tokens, y, pos_pos, Rnode, left_bound, dash_pos, line, node_pos, lines, bra_pos, Rndx, x, unread
    # Function calls: sscanf, warning, my_setdiff, ismember, range, strfind, find, size, textread, eye, min, dot_to_graph, nargout, zeros, findstr, strvcat, max, strmatch, ones, isempty, length, strread, error
    #% [Adj, labels, x, y] = dot_to_graph(filename)
    #% Extract an adjacency matrix, node labels, and layout (nodes coordinates) 
    #% from GraphViz file       http://www.research.att.com/sw/tools/graphviz
    #%
    #% INPUT:  'filename' - the file in DOT format containing the graph layout.
    #% OUTPUT: 'Adj' - an adjacency matrix with sequentially numbered edges; 
    #%     'labels'  - a character array with the names of the nodes of the graph;
    #%          'x'  - a row vector with the x-coordinates of the nodes in 'filename';
    #%          'y'  - a row vector with the y-coordinates of the nodes in 'filename'.
    #%
    #% NOTEs: not guaranted to parse ANY GraphViz file. Debugged on undirected 
    #%       sample graphs from GraphViz(Heawood, Petersen, ER, ngk10_4, process). 
    #%       Complaines about RecursionLimit on huge graphs.
    #%       Ignores singletons (disjoint nodes). Handles loops (arc to self).          
    #% Sample DOT code "ABC.dot", read by [Adj, labels, x, y] = dot_to_graph('ABC.dot')
    #% Plot by    draw_graph(adj>0, labels, zeros(size(x,2),1), x, y);  % from BNT
    #% digraph G {
    #%       A [pos="28,31"];
    #%       B [pos="74,87"];
    #%       A -- B [pos="e,61,71 41,47 46,53 50,58 55,64"];
    #% }
    #%                                                last modified: 24 Feb 2004
    #% by Dr. Leon Peshkin: pesha @ ai.mit.edu | http://www.ai.mit.edu/~pesha 
    #%  & Alexi Savov:  asavov @ wustl.edu |  http://artsci.wustl.edu/~azsavov
    #%  UNCOMMENT, but beware -- SLOW CHECK !!!! 
    #%if ~exist(filename)                % Checks whether the specified file exists.
    #%   error('* * * File does not exist or could not be found. * * *');     return;
    #%end;     
    lines = textread(filename, '%s', 'delimiter', '\n', 'commentstyle', 'c')
    #% Read file into cell array of lines
    dot_lines = strvcat(lines)
    #% ignoring C-style comments
    if isempty(findstr(dot_lines[0,:], 'graph ')):
        #% Is this a DOT file ?
    matcompat.error('* * * File does not appear to be in valid DOT format. * * *')
    return []
    
    Nlns = matcompat.size(dot_lines, 1.)
    #% The number of lines;
    labels = cellarray([])
    unread = np.arange(1., (Nlns)+1)
    #% 'unread' list of lines which has not been examined yet
    edge_id = 1.
    for line_ndx in np.arange(1., (Nlns)+1):
        #% This section sets the adjacency matrix entry A(Lnode,Rnode) = edge_id.
        
    Nvrt = length(labels)
    #% number of vertices we found  [Do we ever have singleton vertices ???]
    #% labels = strvcat(labels); % could convert to the searchable array
    x = np.zeros(1., Nvrt)
    y = np.zeros(1., Nvrt)
    lst_node = 0.
    #% Find node's position coordinates if they are contained in 'filename'.
    for line_ndx in unread:
        #% Look for node's coordiantes among the 'unread' lines.
        
    if np.logical_and(isempty(nonzero(x)), nargout > 2.):
        #% If coordinates were requested, but not found in 'filename'.
    matcompat.warning('File does not contain node coordinates.')
    else:
        x = matdiv(np.dot(.9, x-matcompat.max(x)), range(x))+.05
        #% normalise and push off margins 
        if range(y) == 0.:
            y = np.dot(.5, np.ones(matcompat.size(y)))
        else:
            y = matdiv(np.dot(.9, y-matcompat.max(y)), range(y))+.05
            
        
        
    
    if not matcompat.size(Adj, 1.) == matcompat.size(Adj, 2.):
        #% Make sure Adj is a square matrix. ? 
    Adj = np.dot(np.dot(np.eye(matcompat.max(matcompat.size(Adj)), matcompat.size(Adj, 1.)), Adj), np.eye(matcompat.size(Adj, 2.), matcompat.max(matcompat.size(Adj))))
    
    return [Adj, labels, x, y]