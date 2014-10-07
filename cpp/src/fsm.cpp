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

fsm::fsm(string filename){
	ifstream in(filename.c_str());
	in >> (*this);
	in.close();
}

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

skey_t fsm::get_initial_state() const{
	return initial_state;
}

void fsm::reset(){
	active_state = &find_state(initial_state);
}

bool test_compat(skey_t k1, skey_t k2, const compat_t& compat) {
	if(k1==k2)
		return true;

	const set<skey_t>	&ks1 = compat.find(k1)->second,
						&ks2 = compat.find(k2)->second;

    set<skey_t>::iterator it1 = ks1.find(k2), it2 = ks2.find(k1);
	return (it1 != ks1.end()) || (it2 != ks2.end());
}

bool test_compat(state& s1, state& s2, const compat_t& compat) {
	if(!s1.test_io_map(s2))
		return false;

	const io_map_t& io_map = s1.get_io_map();
	for(io_map_t::const_iterator it = io_map.begin(); it != io_map.end(); it++){
		outpair op2 = s2(it->first);		
		if(op2.output==UNDEFINED)
			continue;

		skey_t	k1 = it->second.state,
				k2 = op2.state;	

		if(!test_compat(k1, k2, compat)){
			// cout << s1.key << ", " << s2.key << " (" << it->first << "):  " << k1 << ", " << k2 << endl;
			return false;
		}
	}
	return true;
}

bool iterate_compat(compat_t& compat, fsm& orig){
	//print_compat(cout);

	bool changed = false;
	for(compat_t::iterator it = compat.begin(); it != compat.end(); it++){		

		set<skey_t>& key_set = it->second;
		for(set<skey_t>::iterator kit = key_set.begin(); kit != key_set.end(); kit++){
			if(!test_compat(orig.find_state(it->first), orig.find_state(*kit), compat)){
//			if(!test_compat(it->first, *kit)){

				changed = true;
				key_set.erase(*kit);
			}
		}
	}

	return changed;
}

compat_t compute_compat(fsm& f){

	compat_t compat;

	const state_map_t& sm = f.get_state_map();

	for(state_map_t::const_iterator it = sm.begin(); it != sm.end(); it++){	
		compat[it->first] = set<skey_t>();
		set<skey_t>& key_set = compat[it->first];
		for(state_map_t::const_iterator jt = sm.begin(); jt != it; jt++)
			key_set.insert(jt->first);
	}

	while(iterate_compat(compat, f));

	return compat;
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
	state_map_t state_map = right.state_map;
	for(state_map_t::const_iterator sit = state_map.begin(); sit != state_map.end(); sit++){
		const state& curr_state = sit->second;
		out << curr_state;
	}
	return out;
}

ostream& operator<<(ostream& out, const compat_t& right){
	for(compat_t::const_iterator it = right.begin(); it != right.end(); it++){
		out << it->first << ": ";
		const set<skey_t>& key_set = it->second;
		for(set<skey_t>::iterator kit = key_set.begin(); kit !=key_set.end(); kit++)
		out << *kit << " ";
		out << endl;
	}
	out << endl;

	return out;
}

void save_dot(string filename, const fsm& right){
	ofstream out(filename.c_str());
	right.save_dot(out);
	out.close();
}


void fsm::save_dot(ostream& out) const{
	const int width  = 80, height = 10;

	string arctxt = "->",
		labeltxt = "[label=\"%s\", color=\"red\", fontname=\"NimbusMonL-Regu\", labeljust=\"r\", fontsize=10]",
		edgeformat = "  %d " + arctxt + " %d " + labeltxt + ";\n";

	out << "digraph G {\n";
	out << "  center=1;\n";
	out << "  size=\"" << width << "," << height << "\";\n";

	int node_index = 0;
	map<skey_t, int> node_indices;
	for(state_map_t::const_iterator it = state_map.begin(); it!=state_map.end(); it++){		
		skey_t found_key = it->first;
		node_indices[found_key] = node_index;
		out << "  " << node_index << " [label=\"" << found_key << "\", shape=\"circle\"];\n";
		node_index++;
	}

	int node1_index = 0;
	for(state_map_t::const_iterator it = state_map.begin(); it!=state_map.end(); it++){
		map<skey_t, string> edge_labels;
		const state& found_state = it->second;
		io_map_t found_map = found_state.io_map;
		for(io_map_t::iterator jt = found_map.begin(); jt!=found_map.end(); jt++){
			outpair op = jt->second;
			stringstream ss;
			ss << jt->first << ":" << op.output << "\\n";
			edge_labels[op.state] += ss.str(); 
		}
		for(map<skey_t, string>::iterator jt = edge_labels.begin(); jt!=edge_labels.end(); jt++){			
			int node2_index = node_indices[jt->first];
			char edgestring[200];		
			sprintf(edgestring, edgeformat.c_str(), node1_index, node2_index, jt->second.c_str());
			out << edgestring;			
		}		
		node1_index++;
	}
	out << "}\n";
}

ostream& operator<<(ostream& out, const cover_t& right){

	for(int i=0; i<right.size(); i++){
		const set<skey_t>& curr = right[i];
		cout << "{ ";

		for(set<skey_t>::const_iterator it = curr.begin(); it!=curr.end(); it++)
			cout << *it << " ";

		cout << "} ";
	}

	return out;
}


typedef map<skey_t, skey_t> clique_map_t;

fsm reduce(fsm& orig, const cover_t& X){
	fsm temp;

	/* Compute name reassignments */
	clique_map_t clique_map;
	for(int i=0; i<X.size(); i++){
		state& new_state = temp.add_state();
		skey_t new_key = new_state.get_key();
		for(set<skey_t>::const_iterator it = X[i].begin(); it!=X[i].end(); it++){
			clique_map[*it] = new_key;
			new_state.add_io_map(orig.find_state(*it));
		}
	}

	state_map_t& sm = temp.get_state_map();
	for(state_map_t::iterator it = sm.begin(); it!=sm.end(); it++){
		state& curr = it->second;
		io_map_t& im = curr.get_io_map();

		for(io_map_t::iterator jt = im.begin(); jt!=im.end(); jt++){
			jt->second.state = clique_map[jt->second.state];
		}
	}

	temp.set_initial_state(clique_map[orig.get_initial_state()]);

	return temp;
}


// bool add_to_clique(skey_t new_entry, set<skey_t>& new_clique, const compat_t& compat){
// 	//cout << "Adding " << new_entry << " to clique (" << new_clique << ").\n";

// 	compat_t::const_iterator mi = compat.find(new_entry);
// 	const set<skey_t>& kc = mi->second;

// 	for(set<skey_t>::iterator it = new_clique.begin(); it!=new_clique.end(); it++){	
// 		compat_t::const_iterator mi = compat.find(*it);

// 		const set<skey_t>& kr = mi->second;

// 		if(kr.find(new_entry)==kr.end() && kc.find(*it)==kc.end())
// 			return false;
// 	}

// 	new_clique.insert(new_entry);
// 	return true;
// }

bool add_to_clique(skey_t new_entry, set<skey_t>& new_clique, const compat_t& compat){
	//cout << "Adding " << new_entry << " to clique (" << new_clique << ").\n";

	compat_t::const_iterator mi = compat.find(new_entry);
	const set<skey_t>& kc = mi->second;

	for(set<skey_t>::iterator it = new_clique.begin(); it!=new_clique.end(); it++){	
		compat_t::const_iterator mi = compat.find(*it);

		const set<skey_t>& kr = mi->second;

		if(kr.find(new_entry)==kr.end() && kc.find(*it)==kc.end())
			return false;
	}

	new_clique.insert(new_entry);
	return true;
}