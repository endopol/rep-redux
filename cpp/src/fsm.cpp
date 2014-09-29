#include "fsm.h"

/* skey_t member functions */
skey_t::skey_t(){
	keynum = -1;
}

skey_t::skey_t(int new_keynum){
	keynum = new_keynum;
}

bool skey_t::operator<(const skey_t& right) const {		
		return keynum<right.keynum;
}

bool skey_t::operator==(const skey_t& right) const {		
		return keynum==right.keynum;
}

istream& operator>>(istream& in, skey_t& right){
	string temp;
	in >> temp;

	istringstream iss(temp.substr(1));

	iss >> right.keynum;

	return in;
}

ostream& operator<<(ostream& out, const skey_t& right){
	out << "q" << right.keynum;

	return out;
}

/* FSM member functions */

state& fsm::add_state(){
	skey_t new_key(state_map.size());
	// cout << "Created key " << new_key << endl;
	return add_state(new_key);
}

state& fsm::add_state(skey_t new_key){
	return state_map.insert(pair<skey_t, state>(new_key, state(new_key))).first->second;
}

state& fsm::find_state(skey_t query_key) {
	return state_map.find(query_key)->second;
}

outpair fsm::operate(in_t input, ostream& out){
	outpair op = (*active_state)(input);
	active_state = &find_state(op.state);

	if(out != NULL){
		out << "(" << input << " -> ";
		if(op.output==UNDEFINED)
			out << "UNDEFINED";
		else 
			out << op.output;
		out << " @ " << op.state << ") ";
}
	return op;
}

vector<outpair> fsm::operate(const vector<in_t>& in_vec, ostream& out){
	vector<outpair> op_vec;
	for(int i=0; i<in_vec.size(); i++){
		in_t curr_in = in_vec[i];
		outpair curr_op = operate(curr_in, out);
		op_vec.push_back(curr_op);	
	}
	out << "\n\n";

	return op_vec;
}

skey_t fsm::get_active_state() const{
	return active_state->key;
}

void fsm::reset(){
	active_state = &find_state(initial_state);
}

bool fsm::test_compat(const state& s1, const state& s2) const{
	if(!s1.test_io_map(s2))
		return false;

	const io_map_t& io_map = s1.io_map;
	for(io_map_t::const_iterator it = io_map.begin(); it != io_map.end(); it++){
		outpair op2 = s2(it->first);		
		if(op2.output==UNDEFINED)
			continue;

		skey_t	k1 = it->second.state,
				k2 = op2.state;	

		if(!test_compat(k1, k2)){
			// cout << s1.key << ", " << s2.key << " (" << it->first << "):  " << k1 << ", " << k2 << endl;
			return false;
		}
	}
	return true;
}

bool fsm::test_compat(skey_t k1, skey_t k2) const{
	if(k1==k2)
		return true;

	const set<skey_t>	&ks1 = compat.find(k1)->second,
						&ks2 = compat.find(k2)->second;

    set<skey_t>::iterator it1 = ks1.find(k2), it2 = ks2.find(k1);
	return (it1 != ks1.end()) || (it2 != ks2.end());
}


bool fsm::iterate_compat(){
	print_compat(cout);

	bool changed = false;
	for(compat_t::iterator it = compat.begin(); it != compat.end(); it++){		

		set<skey_t>& key_set = it->second;
		for(set<skey_t>::iterator kit = key_set.begin(); kit != key_set.end(); kit++){
			if(!test_compat(find_state(it->first), find_state(*kit))){
//			if(!test_compat(it->first, *kit)){

				changed = true;
				key_set.erase(*kit);
			}
		}
	}

	return changed;
}

void fsm::compute_compat(){	

	for(state_map_t::iterator it = state_map.begin(); it != state_map.end(); it++){	
		compat[it->first] = set<skey_t>();
		set<skey_t>& key_set = compat[it->first];
		for(state_map_t::iterator jt = state_map.begin(); jt != it; jt++)
			key_set.insert(jt->first);
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
			skey_t new_key;

			ss >> line_head;
			if(line_head=="state" && ss>>new_key)
				curr_state = &right.add_state(new_key);
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
	fsm::state_map_t state_map = right.state_map;
	for(fsm::state_map_t::const_iterator sit = state_map.begin(); sit != state_map.end(); sit++){
		const state& curr_state = sit->second;
		out << curr_state;
	}
	return out;
}

void fsm::print_compat(ostream& out){
	for(compat_t::iterator it = compat.begin(); it != compat.end(); it++){
		out << it->first << ": ";
		set<skey_t>& key_set = it->second;
		for(set<skey_t>::iterator kit = key_set.begin(); kit !=key_set.end(); kit++)
		out << *kit << " ";
		out << endl;
	}
	out << endl;
}

void fsm::save_dot(ostream& out){
	const int width  = 80, height = 10;

	string arctxt = "->",
		labeltxt = "[label=\"%s\", color=\"red\", fontname=\"NimbusMonL-Regu\", labeljust=\"r\", fontsize=10]",
		edgeformat = "  %d " + arctxt + " %d " + labeltxt + ";\n";

	out << "digraph G {\n";
	out << "  center=1;\n";
	out << "  size=\"" << width << "," << height << "\";\n";

	int node_index = 0;
	map<skey_t, int> node_indices;
	for(state_map_t::iterator it = state_map.begin(); it!=state_map.end(); it++){		
		skey_t found_key = it->first;
		node_indices[found_key] = node_index;
		out << "  " << node_index << " [label=\"" << found_key << "\", shape=\"circle\"];\n";
		node_index++;
	}

	int node1_index = 0;
	for(state_map_t::iterator it = state_map.begin(); it!=state_map.end(); it++){
		map<skey_t, string> edge_labels;
		state& found_state = it->second;
		io_map_t found_map = found_state.io_map;
		for(io_map_t::iterator jt = found_map.begin(); jt!=found_map.end(); jt++){
			outpair op = jt->second;
			stringstream ss;
			ss << jt->first << ":" << op.output << "\\n";
			edge_labels[op.state] += ss.str(); 
		}
		for(map<skey_t, string>::iterator jt = edge_labels.begin(); jt!=edge_labels.end(); jt++){			
			int node2_index = node_indices[jt->first];
			char edgestring[100];		
			sprintf(edgestring, edgeformat.c_str(), node1_index, node2_index, jt->second.c_str());
			out << edgestring;			
		}		
		node1_index++;
	}
	out << "}\n";
}

