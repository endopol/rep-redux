/*
 * =====================================================================================
 *
 *       Filename:  fsm.cpp
 *
 *    Description:  Methods for manipulation, input and output of generic fsms.
 *
 *        Version:  1.0
 *        Created:  10/17/2014 12:46:44 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Joshua Hernandez (jah), endopol@gmail.com
 *   Organization:  UCLA Computer Vision Lab
 *
 * =====================================================================================
 */
#include "fsm.h"


/* #####   SKEY_T MEMBER FUNCTIONS   ################################################ */

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

/* #####   FSM MEMBER FUNCTIONS   ################################################### */

fsm::fsm(string filename){
	ifstream in(filename.c_str());
	in >> (*this);
	in.close();
}

/* STATE MANIPULATION */

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

/* PUBLIC OPERATING INTERFACE */

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

void fsm::reset(){
	active_state = &find_state(initial_state);
}

void fsm::set_initial_state(skey_t new_initial_state){ initial_state = new_initial_state; }

/* FSM ACCESSORS */

state_map_t& fsm::get_state_map(){ return state_map; }
int fsm::get_size() const { return state_map.size(); }

skey_t fsm::get_active_state() const{
	return active_state->key;
}

skey_t fsm::get_initial_state() const{
	return initial_state;
}

/* I/O ROUTINES */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  operator>>(istream&, fsm&)
 *  Description:  Reads in an fsm from an ascii file.  Format:
 *
 *       fsm q0
 *       state q0
 *       	b : 1 @ q0
 *       	c : 1 @ q0
 *       	a : 0 @ q1
 *       state q1
 *       	c : 1 @ q2
 *       	a : 1 @ q3
 *
 * =====================================================================================
 */
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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  operator<<(ostream&, const fsm&)
 *  Description:  Writes an fsm to an ascii file.  Format above.
 * =====================================================================================
 */
ostream& operator<< (ostream& out, const fsm& right){
	out << "fsm " << right.initial_state << endl;
	state_map_t state_map = right.state_map;
	for(state_map_t::const_iterator sit = state_map.begin(); sit != state_map.end(); sit++){
		const state& curr_state = sit->second;
		out << curr_state;
	}
	return out;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  save_dot
 *  Description:  Writes out an FSM in graphviz "dot" format.  See www.graphviz.org for
 *    details.
 * =====================================================================================
 */
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

