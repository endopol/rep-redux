/*
 * =====================================================================================
 *
 *       Filename:  reduce_censi.cpp
 *
 *    Description:  Censi's bit-at-a-time method for reducing a decision tree
 *
 *        Version:  1.0
 *        Created:  10/17/2014 12:52:55 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Joshua Hernandez (jah), endopol@gmail.com
 *   Organization:  UCLA Computer Vision Lab
 *
 * =====================================================================================
 */
#include <sstream>
#include "decision_tree.h"
#include "limits.h"


/*
 * =====================================================================================
 *         Type:  metamap
 *  Description:  A map assigning states from one fsm to "meta-states" of another.
 * =====================================================================================
 */
typedef map<skey_t, skey_t> metamap;
ostream& operator<<(ostream& out, metamap& right){
	for(metamap::const_iterator it = right.begin(); it != right.end(); it++)
		out << it->first << "->" << it->second << "  ";	

	return out;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  test_secondary(metamap&m, state&, state&)
 *  Description:  Determines whether two given states map their inputs onto the same
 *    meta-states.
 * =====================================================================================
 */
bool test_secondary(metamap& mm, state& s1, state& s2){
    io_map_t &im1 = s1.get_io_map(), &im2 = s2.get_io_map();
    for(io_map_t::const_iterator it = im1.begin(); it!=im1.end(); it++){
        in_t in = it->first;           
        io_map_t::iterator found = im2.find(in);
        if(found!=im2.end()){
            skey_t k1 = it->second.state, k2 = found->second.state;
            metamap::iterator mi1 = mm.find(k1), mi2 = mm.find(k2);
            if((mi1!=mm.end()) && (mi2!=mm.end())){
                skey_t nk1 = mi1->second, nk2 = mi2->second;
                if(!(nk1 == nk2))
                    return false;
            }
        }
            
    }
    return true;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: reduce_censi(fsm&, int) 
 *  Description: Map a given decision tree (or a depth-limited fsm) onto a subtree 
 *    with the same policy. 
 * =====================================================================================
 */
fsm reduce_censi(fsm& orig,  int depth){
    fsm temp;
	metamap mm;

	bool verbose = false;
    if(verbose) cout << "\n\nREDUCE CENSI:\n\n";

    // 1. Initialize the meta-map
	state first_state = temp.add_state();
	temp.set_initial_state(first_state.get_key());
	state_map_t& sm = orig.get_state_map();
	for(state_map_t::iterator at = sm.begin(); at!=sm.end(); at++)
		if(at->second.get_io_map().size()>0)
			mm[at->first] = first_state.get_key();

	bool changed = true;
    string change_points;
	while(changed){
		if(verbose) cout << "Map: " << mm << ".\n";

		changed = false;
	
		df_iterator it(&orig, depth);

		int minbp = INT_MAX;
		state* argminbp = NULL;

        // 2. Iterate over pairs of states...
		while(it.get_depth()>0){
			state& is = it.top_state();
			df_iterator jt = it;
			jt.advance();
			while(jt.get_depth()>0){
				state& js = jt.top_state();
								
                // ...selecting pairs of incompatible nodes...
				if(mm[is.get_key()] == mm[js.get_key()] && (!is.test_io_map(js) || !test_secondary(mm, is, js))){					
					changed = true;

                    // ...finding their latest common ancestor... 
					int bp = breakpoint(it, jt);

                    // ...and choosing the least deep such as a split point. 
					if(bp<minbp){
						minbp = bp;
						int split_point = max(1, bp);
						if(it.get_depth()>split_point)
							argminbp = it.get_state(split_point);
						else
							argminbp = jt.get_state(split_point);

                        ostringstream os;
                        os << "it: " << it << endl;
                        os << "it: " << jt << endl;
                        change_points = os.str();
					}
				}

				jt.advance();
			}
			it.advance();
		}

        // 2. Apply new label to the descendants of the split
		if(changed){
			state new_state = temp.add_state();
            if(verbose) cout << change_points << endl;
			if(verbose) cout << "Split \"" << new_state.get_key() << "\" at " << argminbp->get_key() << endl;
			df_iterator bt(&orig, argminbp, depth-minbp);
			for(; bt.get_depth()>0; bt.advance())
				mm[bt.top_state().get_key()] = new_state.get_key();
			if(verbose) cout << mm << "\n\n";
		}
	}

        
	// 3. Induce mappings on metastates
	for(metamap::iterator it = mm.begin(); it!=mm.end(); it++){

		state orig_state = orig.find_state(it->first), 
			  &metastate = temp.find_state(it->second);

		io_map_t& io_map = orig_state.get_io_map();
		for(io_map_t::const_iterator iot = io_map.begin(); iot != io_map.end(); iot++){
			skey_t target = mm[iot->second.state];
			bool replace = (sm.find(target) != sm.end());
			
			outpair op(iot->second.output, target);
			metastate.add_io_map(iot->first, op, replace);
		}
	}

    // 4. Remove illegal mappings
    state_map_t& tm = temp.get_state_map();
   	for(metamap::iterator it = mm.begin(); it!=mm.end(); it++){
        state_map_t::iterator found = tm.find(it->second);
        if(found==tm.end())
            continue;

		state& metastate = found->second;

		io_map_t& t_map = metastate.get_io_map();
		for(io_map_t::iterator iot = t_map.begin(); iot != t_map.end(); iot++){
			skey_t target = iot->second.state;
			if(mm.find(target) == mm.end())
				iot->second.state = metastate.get_key();
		}
	}
   
	return temp;
}
