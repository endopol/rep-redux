#include "decision_tree.h"

bool emulates(fsm& fa, state& a, fsm& fb, state& b, int depth){
	// cout << "depth=" << depth << endl; 

	bool test = b.test_io_map(a);
	if(!test)
		return false;
	
	// cout << "State a:\n" << a;
	// cout << "State b:\n" << b;
	// if(test)
	// 	cout << "Emulates.\n\n";
	// else{
	// 	cout << "Fails to emulate.\n\n";
	// 	return false;
	// }
	
	if(depth>1){
		for(io_map_t::const_iterator it = b.begin(); it!= b.end(); it++){
			outpair opa = a(it->first), opb = b(it->first);
			if(opa.output==UNDEFINED || !emulates(fa, fa.find_state(opa.state), fb, fb.find_state(opb.state), depth-1))
				return false;
		}
	}
	return true;
}

bool emulates(fsm& fa, fsm& fb, int depth){
	return emulates(fa, fa.find_state(fa.get_initial_state()), fb, fb.find_state(fb.get_initial_state()), depth);
}