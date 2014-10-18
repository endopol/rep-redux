#include <list>
#include <algorithm>
#include "decision_tree.h"
#include "reduce_alberto.cpp"


void pvect(vector<skey_t>::iterator begin, vector<skey_t>::iterator end){
	while(begin!=end){
		cout << *begin << " ";
		begin++;
	}
}



bool list_advance(vector<skey_t>::iterator begin, std::vector<skey_t>::iterator end);

int cover_recursive(const compat_t& compat, 
	vector<skey_t>::iterator begin, vector<skey_t>::iterator end,
	int num_cliques, cover_t ct, int lo_bound, int& minmax, cover_t& argmin,
	long& time_count, long max_count);


cover_t cover_exact(fsm& orig, int& lo_bound, long max_count){
	compat_t compat = compute_compat(orig);
	symmetrize(compat);

    set<skey_t> first_clique;
	vector<skey_t> free_keys;
	for(compat_t::iterator it = compat.begin(); it!=compat.end(); it++){
        set<skey_t>& curr = it->second;
        
        if(curr.size()==(compat.size()-1)) 
            // some clever reduction - these states shouldn't have any effect

            first_clique.insert(it->first);
        else
		    free_keys.push_back(it->first);
    }

	cover_t temp, argmin;
	temp.push_back(set<skey_t>());

	cover_t ct = cover_alberto(orig, lo_bound);
	int maxcliques = ct.size();
	if(lo_bound==maxcliques)
		return ct;

	long time_count = 0;
	lo_bound = cover_recursive(compat, free_keys.begin(), free_keys.end(), 1, temp, lo_bound, maxcliques, argmin, time_count, max_count);

	return argmin;
}

int cover_recursive(const compat_t& compat,
	vector<skey_t>::iterator begin, vector<skey_t>::iterator end,
	int num_cliques, cover_t ct, int lo_bound, int& mincliques, cover_t& argmin, 
	long& time_count, long max_count){	

	time_count++;
	if(time_count>=max_count)
		return lo_bound;

	// cout << "Lower bound: " << lo_bound << ", upper bound: " << mincliques << endl;
	// for(vector<skey_t>::iterator it = begin; it!=end; it++)
	// 	cout << *it << " ";
	// cout << endl;

	// cout << ct << "\n\n";

	if(mincliques<=lo_bound)
		return mincliques;

	set<skey_t>* last_clique = &ct.back();
	skey_t last_key = *begin;	

	if(begin==end){
		mincliques = num_cliques;
		argmin = ct;
	}
	else{		
		if(!add_to_clique_symmetric(last_key, *last_clique, compat)){
			num_cliques++;
			ct.push_back(set<skey_t>());
			last_clique = &ct.back();

			last_clique->insert(last_key);
		}

		if(num_cliques<mincliques){
			do{								
				cover_recursive(compat, begin+1, end, num_cliques, ct, lo_bound, mincliques, argmin, time_count, max_count);
			}while(list_advance(begin+1,end));
		}
		
		last_clique->erase(last_clique->find(last_key));
	}
	if(last_clique->size()==0)
		ct.pop_back();

	return mincliques;
}


bool list_advance(vector<skey_t>::iterator begin, std::vector<skey_t>::iterator end){

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

	return true;
}
