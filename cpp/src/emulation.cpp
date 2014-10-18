/*
 * =====================================================================================
 *
 *       Filename:  emulation.cpp
 *
 *    Description:  Routines for testing the equivalence of different policies
 *
 *        Version:  1.0
 *        Created:  10/17/2014 12:46:01 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Joshua Hernandez (jah), endopol@gmail.com
 *   Organization:  UCLA Computer Vision Lab
 *
 * =====================================================================================
 */
#include "decision_tree.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  emulates_recursive
 *  Description:  Assuming the fsm "fa" reacts properly to the first n steps of a given
 *   sequence of inputs, determines whether it reacts to all super-sequences of length
 *   n+1.
 * =====================================================================================
 */
bool emulates_recursive(fsm& fa, state& a, fsm& fb, state& b, int depth){
	
	bool test = b.test_io_map(a);
	if(!test)
		return false;
		
	if(depth>1){
		for(io_map_t::const_iterator it = b.begin(); it!= b.end(); it++){
			outpair opa = a(it->first), opb = b(it->first);
			if(opa.output==UNDEFINED || !emulates_recursive(fa, fa.find_state(opa.state), fb, fb.find_state(opb.state), depth-1))
				return false;
		}
	}
	return true;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  emulates
 *  Description:  Determines whether fsm "fa" obeys all policies, up to a given length, 
 *    required by the fsm "fb".
 * =====================================================================================
 */
bool emulates(fsm& fa, fsm& fb, int depth){
	return emulates_recursive(fa, fa.find_state(fa.get_initial_state()), fb, fb.find_state(fb.get_initial_state()), depth);
}
