#include <list>
#include <algorithm>
#include "decision_tree.h"

template <typename T>
ostream& operator<<(ostream& out, const set<T>& right){
	out << "Set " << right.size() << " items:";
	for(typename set<T>::iterator it = right.begin(); it!=right.end(); it++)
		out << " " << *it << endl;
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

void pvect(vector<skey_t>::iterator begin, vector<skey_t>::iterator end){
	while(begin!=end){
		cout << *begin << " ";
		begin++;
	}
}

bool list_advance(vector<skey_t>::iterator begin, std::vector<skey_t>::iterator end){
	// cout << "Advancing list: ";
	// pvect(begin, end);
	// cout << endl;

	if(begin==end)
		return false;

	vector<skey_t>::iterator it = begin+1, argmin = begin;

	while(it!=end){
		if(*begin<*it && (*it<(*argmin) || (argmin==begin)))
			argmin = it;

		it++;
	}

	if(argmin==begin)
		return false;

	swap(*begin, *argmin);

	sort(begin+1, end);

	// cout << "to : ";
	// pvect(begin, end);
	// cout << endl;

	return true;
}

void cover_recursive(const compat_t& compat,
	vector<skey_t>::iterator begin, vector<skey_t>::iterator end,
	int num_cliques, cover_t ct, int& minmax, cover_t& argmin){	

	set<skey_t>* last_clique = &ct.back();
	skey_t last_key = *begin;	

	if(begin==end){
		minmax = num_cliques;
		argmin = ct;
	}
	else{		
		if(!add_to_clique(last_key, *last_clique, compat)){
			num_cliques++;
			ct.push_back(set<skey_t>());
			last_clique = &ct.back();

			last_clique->insert(last_key);
		}

		//cout << "Num cliques: " << num_cliques << ". " << ct << endl;

		if(num_cliques<=minmax){
			do{								
				cover_recursive(compat, begin+1, end, num_cliques, ct, minmax, argmin);
			}while(list_advance(begin+1,end));
		}
		
		last_clique->erase(last_clique->find(last_key));
	}
	if(last_clique->size()==0)
		ct.pop_back();		
}


cover_t cover_exact(fsm& orig){
	compat_t compat = compute_compat(orig);

	vector<skey_t> free_keys;
	const state_map_t& sm = orig.get_state_map();
	for(state_map_t::const_iterator it = sm.begin(); it!=sm.end(); it++)
		free_keys.push_back(it->first);

	int minmax = orig.get_size();
	cover_t temp(1), argmin;
	cover_recursive(compat, free_keys.begin(), free_keys.end(), 1, temp, minmax, argmin);

	return argmin;
}
