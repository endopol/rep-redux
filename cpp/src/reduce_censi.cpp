#include "decision_tree.h"

#define MAX_DEPTH 4

state& reduce_censi(fsm& orig, state* starting_state, int depth, fsm& redux){
	bool verbose = false;

	string indent = "";
	for(int i=0; i<(MAX_DEPTH-depth); i++)
		indent += "   ";

	state& new_state = redux.add_state();
	if(verbose) cout << indent << "Creating new state \"" << new_state.get_key() << "\" at " << starting_state->get_key() << ".\n";

	for(io_map_t::const_iterator si = starting_state->begin(); si!=starting_state->end(); si++)
		new_state.add_io_map(si->first, outpair(si->second.output, new_state.get_key()), false);

	if(verbose) cout << indent << "Reducing from " << starting_state->get_key() << ", max_depth=" << depth << ".\n";
	df_iterator it(&orig, starting_state, depth);

	bool finished = false;
	while(!finished){
		state& is = it.top_state();

		df_iterator jt = it;
		if(verbose) cout << indent << "Deploying jt.\n";
		bool has_skipped = false;
		while(jt.advance()){
			const state& js = jt.top_state();
			
			if(verbose) cout << indent << "jt at " << jt.top() << endl;	

			if(!is.test_io_map(js)){
				if(verbose) cout << indent << "Split at " << is.get_key()	<< " // " << js.get_key() << ".\n";

				io_map_t::const_iterator top_it = it.top().first;
				if(verbose){
					cout << indent << "it: " << it << endl;
					cout << indent << "jt: " << jt << endl;
				}
				it.step_in();
				state& sub_state = reduce_censi(orig, &it.top_state(), depth-it.get_depth()+1, redux);
			
				new_state.add_io_map(top_it->first, outpair(top_it->second.output, sub_state.get_key()), true);
				if(verbose) cout << indent << "Attached " << it.top_state().get_key() << " to \"" << new_state.get_key() 
								 << "\" as \"" << sub_state.get_key() << "\" by " << top_it->first <<  ":" << top_it->second.output << ".\n";
				
				if(verbose) cout << indent << it << endl;
				if(verbose) cout << indent << "it: skipping from " << it.top() << endl;
				it.step_out();
				
				if(verbose) cout << indent << "it: landing at ";	

				bool stepped = (it.step_over());
				
				if(verbose){
					if(stepped)
						cout << it.top() << endl;
					else
						cout << "END.\n";
				}				

				has_skipped = true;

				break;
			}
			else{
				state& bottom_state = jt.top_state();
				for(io_map_t::const_iterator si = bottom_state.begin(); si!=bottom_state.end(); si++)
					new_state.add_io_map(si->first, outpair(si->second.output, new_state.get_key()), false);

				if(verbose) cout << indent << "Subsumed state " << bottom_state.get_key() << " into \"" << new_state.get_key() << "\".\n";
			}

		}
		if(!has_skipped)
			finished = (!it.advance());
	}
	if(verbose) cout << indent << "Stepping out (depth=" << depth << ").\n\n";

	return new_state;
}

fsm reduce_censi(fsm& orig,  int depth){
	fsm temp;
	state& first = reduce_censi(orig, &(orig.find_state(orig.get_initial_state())), depth, temp);
	temp.set_initial_state(first.get_key());
	return temp;
}