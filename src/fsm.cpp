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
bool state::test_map(in_t new_in, out_t new_out) const{
	io_map_t::const_iterator it = io_map.find(new_in);
	if(it==io_map.end())
		return true;
	
	outpair found = it->second;
	return (found.output == new_out);
}		

bool state::test_map(in_t new_in, outpair new_op) const{
	io_map_t::const_iterator it = io_map.find(new_in);
	if(it==io_map.end())
		return true;

	outpair found_op = it->second;
	return (found_op.output == new_op.output)
			&& (found_op.state == new_op.state);
}

bool state::test_map(io_map_t new_io_map) const{
	for(io_map_t::iterator it = new_io_map.begin(); it != io_map.end(); it++)
		if(test_map(it->first, it->second))
			return false;
	return true;
}

/* add_io_map adds a new mapping or table of mappings */
bool state::add_io_map(io_map_t new_io_map){
	// First try to find inconsistencies
	if(!test_map(new_io_map))
		return false;

	// If none, add mappings all at once
	io_map.insert(new_io_map.begin(), new_io_map.end());
	return true;
}

bool state::add_io_map(in_t new_in, outpair new_outpair){
	if(!test_map(new_in, new_outpair))
		return false;

	io_map[new_in] = new_outpair;
	return true;
}

bool state::add_io_map(in_t new_in, out_t new_out){
	state* new_state = base->add_state();
	
	add_io_map(new_in, outpair(new_out, new_state->key));
}

ostream& operator<< (ostream& out, const state& right){
	out << "state " << right.key << endl;
	const io_map_t& curr_map = right.io_map;
	for(io_map_t::const_iterator it = curr_map.begin(); it != curr_map.end(); it++)
		out << "\t" << it->first << " : " << it->second.output << " @ " << it->second.state << endl;	
}


/* FSM member functions */
key_t fsm::get_new_key() const{
	int key_num = state_map.size();

	string key_name;
	do{
		stringstream ss;
		ss << "q" << key_num;
		key_num++;
	}while(state_map.find(key_name)!=state_map.end());

	return (key_t) key_name;
}


state* fsm::add_state(){
	key_t new_key = get_new_key();
	return add_state(new_key);
}

state* fsm::add_state(key_t new_key){
	state* new_state_ptr = 	
		&(state_map.insert(pair<key_t, state>(new_key, state(this, new_key))).first->second);

	return new_state_ptr;	
}

state* fsm::find_state(key_t query_key) {
	return &(state_map.find(query_key)->second);
}


outpair state::map(in_t input){
	io_map_t::const_iterator it = io_map.find(input);

	if(it!=io_map.end())
		return it->second;
	else
		return outpair(UNDEFINED, key);
}

outpair fsm::operate(in_t input, ostream& out){
	outpair op = active_state->map(input);
	active_state = find_state(op.state);

	out << "(" << input << " -> ";
	if(op.output==UNDEFINED)
		out << "UNDEFINED";
	else 
		out << op.output;
	out << " @ " << op.state << ") ";
	return op;
}

vector<outpair> fsm::operate(const vector<in_t>& in_vec, ostream& out){
	vector<outpair> op_vec;
	for(int i=0; i<in_vec.size(); i++){
		in_t curr_in = in_vec[i];
		outpair curr_op = operate(curr_in, out);
		op_vec.push_back(curr_op);	
	}
	out << endl;

	return op_vec;
}

key_t fsm::get_active_state() const{
	return active_state->key;
}

void fsm::reset(){
	active_state = find_state(initial_state);
}

bool compatible(const state& s1, const state& s2){
	for(io_map_t::iterator it = s1.io_map.begin(); it != s1.io_map.end(); it++)
		if(!s2.test_map(it->first, it->second))
			return false;

	return true;
}

bool fsm::iterate_compat(){
	for(compat_t::iterator it = compat.begin(); it != compat.end(); it++){		

		set& key_set = it->second;

		for(set<key>::iterator kit = key_set.begin(); kit != key_set.end(); kitt++){
			if(!compatible(*find_state(it->first), *find_state(*kit)))
				key_set.remove(*kit);
		}
	}
}

void fsm::compute_compat(){	

	for(set_map_t::iterator it = set_map.begin(); it != set_map.end(); it++)
		compat.[it->first] = set<key>;

	for(compat_t::iterator it = compat.begin(); it != compat.end(); it++){
		
		set<key> key_set = it->second;

		if(compat_t::iterator jt = compat.begin(); jt != it; jt++){
			key_set.insert(*jt);
		}
	}

	while(iterate_compat());
}


/* I/O routines */

istream& operator>> (istream& in, fsm& right){
	string temp, new_line, line_head;
	if(in>>temp && temp=="fsm" && in>>right.initial_state){
		in.ignore();

		state* curr_state;
		while(getline(in, new_line)){
			stringstream ss(new_line);
			
			in_t new_in;
			out_t new_out;
			key_t new_key;

			ss >> line_head;
			if(line_head=="state" && ss>>new_key)
				curr_state = right.add_state(new_key);
			else{
				string in_string, out_string, key_string;

				stringstream ms(new_line);
				if(		getline(ms, in_string, ':') && 
						getline(ms, out_string, '@') &&	
						getline(ms, key_string)){
					
					stringstream in_stream(in_string), 
								 out_stream(out_string), 
								 key_stream(key_string);

					if(in_stream>>new_in && out_stream >> new_out && key_stream >> new_key)
						curr_state->add_io_map(new_in, outpair(new_out, new_key));
				}
			}
		}
	}

	return in;
}

ostream& operator<< (ostream& out, const fsm& right){
	out << "fsm " << right.initial_state << endl;
	state_map_t state_map = right.state_map;
	for(state_map_t::const_iterator sit = state_map.begin(); sit != state_map.end(); sit++){
		const state& curr_state = sit->second;
		out << curr_state;
	}
	return out;
}
