#include "reorder.c"
#include "cliquer.c"
#include "graph.c"
//#include "cl.c"

#include "decision_tree.h"

static set_t *clique_list;
static int clique_count=0;
static int clique_list_size=0;
static int *(*reorder)(graph_t *, boolean)=reorder_by_default;
boolean record_clique_func(set_t s,graph_t *g,clique_options *opts);

// convert compat_t to graph
graph_t* new_graph(compat_t& ct, vector<skey_t>& keylist){
	graph_t* g = graph_new(ct.size());

	int i=0;
	for(compat_t::iterator it = ct.begin(); it!=ct.end(); it++){

		set<skey_t>& curr_set = it->second;
		int j=0;
		for(compat_t::const_iterator jt = ct.begin(); jt!=it; jt++){
			if((curr_set.find(jt->first)==curr_set.end()))
				GRAPH_ADD_EDGE(g, i, j);
			j++;
		}
		keylist.push_back(it->first);
		i++;
	}
	return g;
}

set<skey_t> max_indep_set(compat_t& compat){

	clique_options *opts;
	opts= (clique_options*) malloc(sizeof(clique_options));
	opts->time_function=NULL;//clique_print_time;
	opts->reorder_function=reorder;
	opts->reorder_map=NULL;
	opts->user_function=NULL;//record_clique_func;
	opts->user_data=NULL;
	opts->clique_list=NULL;
	opts->clique_list_length=0;

	vector<skey_t> keylist;
	graph_t* g = new_graph(compat, keylist);
	set_t X = clique_unweighted_find_single(g,0,0,FALSE,opts);

	set<skey_t> new_clique;

	// cout << "keylist: ";
	// for(int i=0; i<keylist.size(); i++)
	// 	cout << keylist[i] << " ";
	// cout << endl;


	// set_print(X);

	int i=-1;
	while((i=set_return_next(X,i))>=0){
		new_clique.insert(keylist[i]);
	}
	

	set_free(X);
	graph_free(g);
	free(opts);

	return new_clique;
}

// find  maximum independent set
cover_t cover_alberto(compat_t compat, int& lo_bound){
	set<skey_t> clique = max_indep_set(compat); 
	
	lo_bound = clique.size();

	list<skey_t> free_keys;
	for(compat_t::iterator it = compat.begin(); it!=compat.end(); it++)
		if(clique.find(it->first)==clique.end())
			free_keys.push_back(it->first);

	// cout << "free_keys: " << free_keys << endl;

	symmetrize(compat);

	cover_t X;
	for(set<skey_t>::iterator it = clique.begin(); it!=clique.end(); it++){
		skey_t next_key = *it;

		// cout << "next_key: " << next_key << endl;

		X.push_back(set<skey_t>());
		set<skey_t>& new_clique = X.back();
		new_clique.insert(next_key);

		for(list<skey_t>::iterator jt = free_keys.begin(); jt!=free_keys.end(); jt++){
			if(add_to_clique_symmetric(*jt, new_clique, compat)){
				list<skey_t>::iterator erase_place = jt++;
				free_keys.erase(erase_place);
			}

			// cout << " *jt: " << *jt <<  " free_keys: " << free_keys << endl;
		}
	}

	return X;
}

// find  maximum independent set
cover_t cover_alberto(fsm& orig, int& lo_bound){
	return cover_alberto(compute_compat(orig), lo_bound);
}



/*
 * Records a clique into the clique list using dynamic allocation.
 * Used as opts->user_function.
 */
boolean record_clique_func(set_t s,graph_t *g,clique_options *opts) {
	if (clique_count>=clique_list_size) {
		clique_list= (setelement**) realloc(clique_list,(clique_list_size+512) * 
				    sizeof(set_t));
		clique_list_size+=512;
	}
	clique_list[clique_count]=set_duplicate(s);
	clique_count++;
	return TRUE;
}



// greedily build cliques from each
