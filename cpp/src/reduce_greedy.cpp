#include "fsm.h"
#include "Graph.h"
#include <algorithm>

vector<skey_t> read_keylist(const compat_t& ct){
	vector<skey_t> keylist;
	for(compat_t::const_iterator it = ct.begin(); it!=ct.end(); it++)
		keylist.push_back(it->first);

	return keylist;
}

cover_t apply_keylist(const vector<vector<int> >& vv, const vector<skey_t>& keylist){
 	cover_t ct(vv.size());

	for(int i=0; i<vv.size(); i++)
		for(int j=0; j<vv[i].size(); j++)
			ct[i].insert(keylist[vv[i][j]]);

	return ct;
}

cover_t cover_greedy(const compat_t& ct){

	vector<skey_t> keylist = read_keylist(ct);
	adj_t A = compute_adjacency(ct);
	
	Graph G(A);
	Clique X(G);

 	vector<vector<int> > cv = X.getCliquePartitionList();

	return apply_keylist(cv, keylist);
}

cover_t cover_greedy(fsm& f){
	return cover_greedy(compute_compat(f));
}

