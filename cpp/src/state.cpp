#include "fsm.h"

state::state(skey_t new_key){
	key = new_key;
}

state::state(skey_t new_key, io_map_t new_io_map){
	key = new_key;
	io_map = new_io_map;
}

/* test_map tests whether a new mapping or table of mappings interferes with existing ones */
bool state::test_io_map(in_t new_in, outpair new_op) const{
	io_map_t::const_iterator it = find(new_in);
	if(it==io_map.end())
		return true;

	outpair found_op = it->second; 
	return found_op.output == new_op.output;
}

// bool state::test_io_map(const state& s) const{
// 	const io_map_t& new_map = s.io_map;
// 	for(io_map_t::const_iterator it = new_map.begin(); it != new_map.end(); it++)
// 		if(!test_io_map(it->first, it->second))
// 			return false;
// 	return true;
// }


bool state::test_io_map(const state& s) const{
	for(io_map_t::const_iterator it = io_map.begin(); it != io_map.end(); it++)
		if(!s.test_io_map(it->first, it->second))
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

bool state::add_io_map(in_t new_in, outpair new_outpair, bool replace){
	if(!test_io_map(new_in, new_outpair))
		return false;

	if(io_map.find(new_in)!=io_map.end() && !replace)
		return true;

	io_map[new_in] = new_outpair;
	return true;
}

bool state::add_io_map(in_t new_in, outpair new_outpair){
	return add_io_map(new_in, new_outpair, true);	
}

ostream& operator<< (ostream& out, const state& right){	
	out << "state " << right.key << endl;
	const io_map_t& curr_map = right.io_map;
	for(io_map_t::const_iterator it = curr_map.begin(); it != curr_map.end(); it++){
		in_t input = it->first;
		out_t output = it->second.output;
		skey_t key = it->second.state;
		out << "\t" << input << " : " << output << " @ " << key << endl;	
	}
	return out;
}

outpair state::operator()(in_t input) const{
	io_map_t::const_iterator it = find(input);

	if(it!=io_map.end())
		return it->second;
	else
		return outpair(UNDEFINED, key);
}

io_map_t::const_iterator state::find(in_t input) const{
	return io_map.find(input);
}

io_map_t::const_iterator state::begin() const{
	return io_map.begin();
}

io_map_t::const_iterator state::end() const{
	return io_map.end();
}
