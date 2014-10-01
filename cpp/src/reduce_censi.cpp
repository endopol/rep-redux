#include "decision_tree.h"

#define MAX_DEPTH 4

void reduce_censi(const fsm& orig, state* starting_state, int depth, fsm& redux){
	
	string indent = "";
	for(int i=0; i<(MAX_DEPTH-depth); i++)
		indent += "   ";

	bool verbose = false;

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
				cout << indent << "Split at " << is.get_key()	<< " // " << js.get_key() << ".\n";

				it.step_in();
				reduce_censi(orig, &it.top_state(), depth-it.get_depth()+1);
				
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
		}
		if(!has_skipped)
			finished = (!it.advance());
	}
	if(verbose) cout << indent << "Stepping out (depth=" << depth << ").\n\n";

	return;
}

fsm reduce_censi(const fsm& orig,  int depth){
	fsm temp;
	reduce_censi(orig, &(orig.find_state(temp.get_initial_state())), depth, temp);
	return temp;
}