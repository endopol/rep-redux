#include "fsm.h"

state::state(fsm* new_base, key_t new_key){
	base = new_base;
	key = new_key;
}

state::state(fsm* new_base, key_t new_key, io_map_t new_io_map){
	base = new_base;
	key = new_key;
	io_map = new_io_map;
}

/* test_map tests whether a new mapping or table of mappings interferes with existing ones */
bool state::test_io_map(in_t new_in, outpair new_op) const{
	io_map_t::const_iterator it = io_map.find(new_in);
	if(it==io_map.end())
		return true;

	outpair found_op = it->second; 
	return found_op.output == new_op.output;
}

bool state::test_io_map(const state& s) const{
	const io_map_t& new_map = s.io_map;
	for(io_map_t::const_iterator it = new_map.begin(); it != new_map.end(); it++)
		if(!test_io_map(it->first, it->second))
			return false;
	return true;
}

/* add_io_map adds a new mapping or table of mappings */
bool state::add_io_map(const state& s){
	// First try to find inconsistencies
	if(!test_io_map(s))
		return false;

	const io_map_t& new_io_map = s.io_map;
	// If none, add mappings all at once
	io_map.insert(new_io_map.begin(), new_io_map.end());
	return true;
}

bool state::add_io_map(in_t new_in, outpair new_outpair){
	if(!test_io_map(new_in, new_outpair))
		return false;

	io_map[new_in] = new_outpair;
	return true;
}

ostream& operator<< (ostream& out, const state& right){	
	out << "state " << right.key << endl;
	const io_map_t& curr_map = right.io_map;
	for(io_map_t::const_iterator it = curr_map.begin(); it != curr_map.end(); it++)
		out << "\t" << it->first << " : " << it->second.output << " @ " << it->second.state << endl;	
}

outpair state::operator()(in_t input) const{
	io_map_t::const_iterator it = io_map.find(input);

	if(it!=io_map.end())
		return it->second;
	else
		return outpair(UNDEFINED, key);
}


