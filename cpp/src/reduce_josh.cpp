#include <list>
#include "decision_tree.h"

// template <typename T>
// ostream& operator<<(ostream& out, const set<T>& right)
// {
// 	out << "Set " << right.size() << " items:";
// 	for(typename set<T>::iterator it = right.begin(); it!=right.end(); it++)
// 		out << " " << *it;
// 	return out;
// }



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
