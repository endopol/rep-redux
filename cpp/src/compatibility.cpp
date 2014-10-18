/*
 * =====================================================================================
 *
 *       Filename:  compatibility.cpp
 *
 *    Description:  Functions for dealing with compatibility matrices and cliques
 *
 *        Version:  1.0
 *        Created:  10/17/2014 12:58:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Joshua Hernandez (jah), endopol@gmail.com
 *   Organization:  UCLA Computer Vision Lab
 *
 * =====================================================================================
 */
#include "fsm.h"


/* #####   COMPATIBILITY FUNCTIONS   ################################################ */

bool test_compat(skey_t k1, skey_t k2, const compat_t& compat) {
	if(k1==k2)
		return true;

	const set<skey_t>	&ks1 = compat.find(k1)->second,
						&ks2 = compat.find(k2)->second;

    set<skey_t>::iterator it1 = ks1.find(k2), it2 = ks2.find(k1);
	return (it1 != ks1.end()) || (it2 != ks2.end());
}

bool test_compat(state& s1, state& s2, const compat_t& compat) {
	if(!s1.test_io_map(s2))
		return false;

	const io_map_t& io_map = s1.get_io_map();
	for(io_map_t::const_iterator it = io_map.begin(); it != io_map.end(); it++){
		outpair op2 = s2(it->first);		
		if(op2.output==UNDEFINED)
			continue;

		skey_t	k1 = it->second.state,
				k2 = op2.state;	

		if(!test_compat(k1, k2, compat)){
			// cout << s1.key << ", " << s2.key << " (" << it->first << "):  " << k1 << ", " << k2 << endl;
			return false;
		}
	}
	return true;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  iterate_compat, compute_compat
 *  Description:  Iterates, up to stationarity, the first-order compatibility relations
 *    of an fsm.
 * =====================================================================================
 */
bool iterate_compat(compat_t& compat, fsm& orig){

	bool changed = false;
	for(compat_t::iterator it = compat.begin(); it != compat.end(); it++){		

		set<skey_t>& key_set = it->second;
		for(set<skey_t>::iterator kit = key_set.begin(); kit != key_set.end(); kit++){
			if(!test_compat(orig.find_state(it->first), orig.find_state(*kit), compat)){

				changed = true;
				key_set.erase(*kit);
			}
		}
	}
	return changed;
}

compat_t compute_compat(fsm& f){

	compat_t compat;

	const state_map_t& sm = f.get_state_map();

	for(state_map_t::const_iterator it = sm.begin(); it != sm.end(); it++){	
		compat[it->first] = set<skey_t>();
		set<skey_t>& key_set = compat[it->first];
		for(state_map_t::const_iterator jt = sm.begin(); jt != it; jt++)
			key_set.insert(jt->first);
	}

	while(iterate_compat(compat, f));

	return compat;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  symmetrize(compat_t&) 
 *  Description:  Makes sure that the compatibility relationship described by a given
 *    compatibility matrix is symmetric.
 * =====================================================================================
 */
void symmetrize(compat_t& compat){
	for(compat_t::iterator it = compat.begin(); it!=compat.end(); it++){
		skey_t curr_key = it->first;
		set<skey_t>& curr_set = it->second;
		for(set<skey_t>::iterator jt = curr_set.begin(); jt!=curr_set.end(); jt++){
			set<skey_t>& neighbor_set = compat[*jt];

			neighbor_set.insert(curr_key);
		}
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  density(compat_t&) 
 *  Description:  Computes the density (ratio of compatible pairs to total pairs) of a
 *    compatibility matrix.
 * =====================================================================================
 */
float density(compat_t& compat){
	symmetrize(compat);

	int count = 0;
	for(compat_t::const_iterator it = compat.begin(); it!=compat.end(); it++){
		count+= it->second.size();
	}

	return count/((float) compat.size()*compat.size());
}

ostream& operator<<(ostream& out, const compat_t& right){
	for(compat_t::const_iterator it = right.begin(); it != right.end(); it++){
		out << it->first << ": ";
		const set<skey_t>& key_set = it->second;
		for(set<skey_t>::iterator kit = key_set.begin(); kit !=key_set.end(); kit++)
		out << *kit << " ";
		out << endl;
	}
	out << endl;

	return out;
}


ostream& operator<<(ostream& out, set<skey_t>& right){
	for(set<skey_t>::const_iterator it = right.begin(); it != right.end(); it++){
        out << *it << " ";
	}
	out << endl;

	return out;
}



/* #####   CLIQUE-COVER FUNCTIONS   ################################################# */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  add_to_clique, add_to_clique_symmetric
 *  Description:  Attempts to add a given state to a given set of states, with success
 *    if and only if the first state is compatible with all other states.
 * =====================================================================================
 */
bool add_to_clique_symmetric(skey_t new_entry, set<skey_t>& new_clique, const compat_t& compat){
	//cout << "Adding " << new_entry << " to clique (" << new_clique << ").\n";

	for(set<skey_t>::iterator it = new_clique.begin(); it!=new_clique.end(); it++){	
		compat_t::const_iterator mi = compat.find(*it);

		const set<skey_t>& kr = mi->second;

		if(kr.find(new_entry)==kr.end())
			return false;
	}

	new_clique.insert(new_entry);
	return true;
}

bool add_to_clique(skey_t new_entry, set<skey_t>& new_clique, const compat_t& compat){
	//cout << "Adding " << new_entry << " to clique (" << new_clique << ").\n";

	compat_t::const_iterator mi = compat.find(new_entry);
	const set<skey_t>& kc = mi->second;

	for(set<skey_t>::iterator it = new_clique.begin(); it!=new_clique.end(); it++){	
		compat_t::const_iterator mi = compat.find(*it);

		const set<skey_t>& kr = mi->second;

		if(kr.find(new_entry)==kr.end() && kc.find(*it)==kc.end())
			return false;
	}

	new_clique.insert(new_entry);
	return true;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  reduce(fsm&, cover_t&)
 *  Description:  Given a clique partition of its state graph, condenses the graph into a
 *    corresponding subgraph.
 * =====================================================================================
 */
typedef map<skey_t, skey_t> clique_map_t;

fsm reduce(fsm& orig, const cover_t& X){
	fsm temp;

	/* Compute name reassignments */
	clique_map_t clique_map;
	for(int i=0; i<X.size(); i++){
		state& new_state = temp.add_state();
		skey_t new_key = new_state.get_key();
		for(set<skey_t>::const_iterator it = X[i].begin(); it!=X[i].end(); it++){
			clique_map[*it] = new_key;
			new_state.add_io_map(orig.find_state(*it));
		}
	}

    /* Collapse to reassigned states */
	state_map_t& sm = temp.get_state_map();
	for(state_map_t::iterator it = sm.begin(); it!=sm.end(); it++){
		state& curr = it->second;
		io_map_t& im = curr.get_io_map();

		for(io_map_t::iterator jt = im.begin(); jt!=im.end(); jt++){
			jt->second.state = clique_map[jt->second.state];
		}
	}

	temp.set_initial_state(clique_map[orig.get_initial_state()]);

	return temp;
}

/* I/O ROUTINES */

ostream& operator<<(ostream& out, const cover_t& right){
	for(int i=0; i<right.size(); i++){
		const set<skey_t>& curr = right[i];
		cout << "{ ";

		for(set<skey_t>::const_iterator it = curr.begin(); it!=curr.end(); it++)
			cout << *it << " ";
		cout << "} ";
	}
	return out;
}

/* #####   COVER AND COMPATIBILITY FUNCTIONS   ###################################### */

 bool verify_clique(compat_t& ct, set<skey_t>& s){
     for(set<skey_t>::const_iterator it = s.begin(); it!=s.end(); it++){
         skey_t k1 = *it;
         set<skey_t> curr = ct[k1];

         for(set<skey_t>::const_iterator jt = it; jt!=s.end(); jt++){
            if(it==jt)
                continue;
            skey_t k2 = *jt;
            if(curr.find(k2)==curr.end())
                return false;
         }
     }

     return true;
 }

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  verify_cover(compat_t&, cover_t&)
 *  Description:  Determines whether a clique cover is valid w/r/t a given compatibility
 *    matrix, i.e. it partitions the states of the matrix, and every one of its sets
 *    induces a clique in the set graph.
 * =====================================================================================
 */
bool verify_cover(compat_t& ct, cover_t& X){
    
    set<skey_t> u;

    int cover_size = 0;
	for(int i=0; i<X.size(); i++){

        if(!verify_clique(ct, X[i]))
            return false;
        
	    cover_size += X[i].size();
        u.insert(X[i].begin(), X[i].end());
	}
    
    return ((cover_size==ct.size()) && (cover_size==u.size()));
}

