#include <list>
#include "decision_tree.h"

template <typename T>
ostream& operator<<(ostream& out, const set<T>& right)
{
	out << "Set " << right.size() << " items:";
	for(typename set<T>::iterator it = right.begin(); it!=right.end(); it++)
		out << " " << *it;
	return out;
}

bool add_to_clique(skey_t new_entry, set<skey_t>& new_clique, const compat_t& compat);

cover_t cover_josh(const compat_t& ct, fsm& orig){
	cover_t X;

	list<skey_t> free_keys;
	
	const state_map_t& sm = orig.get_state_map();
	for(state_map_t::const_iterator it = sm.begin(); it!=sm.end(); it++)
		free_keys.push_back(it->first);

	//cout << free_keys << endl;

	while(!free_keys.empty()){
			
		X.push_back(set<skey_t>());
		set<skey_t>& new_clique = X.back();

		vector<list<skey_t>::iterator> erase_list;

		for(list<skey_t>::iterator it = free_keys.begin(); it!=free_keys.end(); it++)

			if(add_to_clique(*it, new_clique, ct))
				erase_list.push_back(it);

		for(int i=0; i<erase_list.size(); i++)
			free_keys.erase(erase_list[i]);
	}

	return X;
}

cover_t cover_josh(fsm& orig){
	return cover_josh(compute_compat(orig), orig);
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