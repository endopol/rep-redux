#include <list>
#include <algorithm>
#include "decision_tree.h"

template <typename T>
ostream& operator<<(ostream& out, const set<T>& right){
	out << "Set " << right.size() << " items:";
	for(typename set<T>::iterator it = right.begin(); it!=right.end(); it++)
		out << " " << *it;
	return out;
}

template <typename T>
ostream& operator<<(ostream& out, const list<T>& right){
	out << "Set " << right.size() << " items:";
	for(typename list<T>::const_iterator it = right.begin(); it!=right.end(); it++)
		out << " " << *it;
	return out;
}

bool add_to_clique(skey_t new_entry, set<skey_t>& new_clique, const compat_t& compat);

fsm reduce_josh_exhaustive(fsm& orig){
	int min_size = orig.get_size();

	orig.compute_compat();

	list<skey_t> free_keys_orig;
	const state_map_t& sm = orig.get_state_map();
	for(state_map_t::const_iterator it = sm.begin(); it!=sm.end(); it++)
		free_keys_orig.push_back(it->first);

	fsm best;
	while(next_permutation(free_keys_orig.begin(), free_keys_orig.end())){

		cout << "Permutation: " << free_keys_orig << endl;

		list<skey_t> free_keys = free_keys_orig;

		fsm temp;
		temp.set_initial_state(*free_keys.begin());

		map<skey_t, skey_t> clique_map;

		//cout << free_keys << endl;

		while(!free_keys.empty()){
			state& new_state = temp.add_state();
			
			set<skey_t> new_clique;
			vector<list<skey_t>::iterator> erase_list;		

			for(list<skey_t>::iterator it = free_keys.begin(); it!=free_keys.end(); it++)
				if(add_to_clique(*it, new_clique, orig.get_compat())){
					clique_map[*it] = new_state.get_key();
					new_state.add_io_map(orig.find_state(*it));
					erase_list.push_back(it);
				}

			for(vector<list<skey_t>::iterator>::iterator it = erase_list.begin(); it!=erase_list.end(); it++)
				free_keys.erase(*it);	/* code */
		}

		state_map_t& tm = temp.get_state_map();
		for(state_map_t::iterator it = tm.begin(); it!=tm.end(); it++){
			io_map_t& im = it->second.get_io_map();		
			for(io_map_t::iterator jt = im.begin(); jt!=im.end(); jt++){
				skey_t& transition = jt->second.state;
				transition = clique_map[transition];
			}
		}

		if(temp.get_size()<min_size){
			min_size = temp.get_size();
			best = temp;
		}
	}

	return best;
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