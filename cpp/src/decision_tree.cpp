#include "decision_tree.h"

ostream& operator<<(ostream& out, const trace& right){
	if(right.second!=NULL)
		out << right.second->get_key();
	else
		out << "NULL";

	out << "(" << right.first->first << ")";
	return out;
}

decision_tree::decision_tree(fsm in, int depth){
	state* new_state = &(add_state());
	initial_state = new_state->get_key();
	df_iterator tt(this, depth);

	df_iterator it(&in, depth);

	do{			
		while(tt.get_depth() > it.get_depth())
			tt.step_out();

		new_state = &(add_state());

		io_map_t::const_iterator ii = it.top().first;
		in_t input = ii->first;
		out_t output = ii->second.output;
		
		state& old_state = tt.top_state();
		// cout << "Got state " << ii->second.state << ".\n";
		// cout << "Attaching " << new_state->get_key() << " to " << old_state->get_key() 
		// 	<< " (" << input << ":" << output << " -> " << new_state->get_key() << ") " << endl;		
		old_state.add_io_map(input, outpair(output, new_state->get_key()));
		tt.step_in(input);

	}while(it++);

}

void decision_tree::write_tree(ostream& out, int depth){
	df_iterator it(this, depth);

	int new_depth, old_depth = 1; 
	
	bool finished = false;
	out << "{ ";
	while(!finished){
		const state& curr_state = it.top_state();
		io_map_t::const_iterator curr = it.top().first;
		out << curr->first << ":" << curr->second.output << ""; //<< ":" << it.get_depth() << " ";
		// cout << "Writing " << curr->first << ":" << curr->second.output << "" << endl;
		out.flush();

		if(!it++)
			finished = true;

		new_depth = it.get_depth();

		//out << " " << old_depth << "-" << new_depth << " ";

		if(new_depth > old_depth)
			out << " { ";		
		while(new_depth < old_depth){
			out << " }";
			old_depth--;
		}
		if(new_depth == old_depth)
			out << "; ";

		old_depth = new_depth;
	}
	out << endl;
}


int count(const string& str, char c){
	return count(str.begin(), str.end(), c);
}

void trim(string& s, const char* delim)
{
   size_t p = s.find_first_not_of(delim);
   s.erase(0, p);

   p = s.find_last_not_of(delim);
   if (string::npos != p)
      s.erase(p+1);
}

void decision_tree::read_tree(istream& in, int read_depth){

	state* old_state = &(add_state());
	initial_state = old_state->get_key();
	df_iterator it(this, read_depth);	

	int depth = 0;
	string newline;
	while(getline(in, newline, ';')){

		istringstream ss(newline);

		int upcount;
		string newtoken;
		while(getline(ss, newtoken, '{')){				
			
			upcount = count(newtoken, '}');
			trim(newtoken, "} \t\n");

			if(depth<=read_depth && newtoken.length()>0){

				old_state = &it.top_state();

				state* new_state = &(add_state());

				int breakpoint = newtoken.find(':');

				istringstream is (newtoken.substr(0,breakpoint));
				istringstream os (newtoken.substr(breakpoint+1));

				in_t new_input;
				out_t new_output;

				is >> new_input;
				os >> new_output;

				bool success = old_state->add_io_map(new_input, outpair(new_output, new_state->get_key()));

				//cout << "New state: " << (*new_state);
				//cout << "Old state: " << (*old_state);;

				it.step_in(new_input);
			}
			depth++;

			depth -= upcount;
		}		
		depth--;

		//cout << "upcount=" << upcount << ", depth=" << depth << endl;
		while(it.get_depth()>depth)
			it.step_out();		

		//cout << it << endl;
	}
}

df_iterator::df_iterator(fsm* new_base, int new_depth){
	base = new_base;
	max_depth = new_depth;
	root = &(base->find_state(base->get_initial_state()));

	stack.push_back(trace(root->begin(), root));
}

df_iterator::df_iterator(fsm* new_base, state* first_state, int new_depth){
	base = new_base;
	max_depth = new_depth;
	root = first_state;

	stack.push_back(trace(root->begin(), root));	
}

bool df_iterator::step_in(){
	if(get_depth()==0)
		return false;

	const state& s = top_state();
	io_map_t::const_iterator it = top().first;
	if(it==s.end())
		return false;

	return step_in(it->first);
}

bool df_iterator::step_in(in_t in){	
	trace& top_trace = top();
	state* s = top_trace.second;
	io_map_t::const_iterator it = s->find(in);
	if(it==s->end())
		return false;
	
	skey_t next_key = it->second.state;
	state* next = &(base->find_state(next_key));
	stack.push_back(trace(next->begin(), next));
	return (next->get_size()>0);
}

void df_iterator::step_out(){
	stack.pop_back();
}

int df_iterator::get_depth() const{
	return stack.size();
}

trace& df_iterator::top(){
	if(stack.size()==0)
		return (* (trace*) NULL);
	
	return stack.back();
}

state& df_iterator::top_state(){
	trace& top_trace = top();
	if(&top_trace==NULL)
		return (* (state*) NULL);
	else return *(top_trace.second);
}

void df_iterator::split(const df_iterator& right){
	df_iterator ix(base, max_depth);

	int common_depth = min(get_depth(), right.get_depth());


	vector<trace> new_stack;
		

	for(int i=0; i<common_depth && stack[i]==right.stack[i]; i++)
		new_stack.push_back(stack[i]);

	stack = new_stack;
}

bool df_iterator::operator++(){
	// cout << "Started at " << top_state()->get_key() 
	// 	 	<< " (" << top().first->first << ":" << top().first->second.output << ").\n";

	if(get_depth()<max_depth && step_in()){				
		// cout << "Stepped into " << top_state()->get_key() 
		//  	<< " (" << top().first->first << ":" << top().first->second.output << ").\n";
		return true;
	}
	
	while(get_depth()>0){
		
		bool tiny = (top_state().get_size()==0);
		if(!tiny)
			(top().first)++;

		if(tiny || top().first == top_state().end()) {		
			step_out();

			if(&top()==NULL)
				return false;
		}
		else
			break;
	}

	// cout << "Stepped onto " << top_state()->get_key()
	// 	<< " (" << top().first->first << ":" << top().first->second.output << ").\n";
	return true;
}

bool df_iterator::operator++(int x){ ++(*this); }

bool df_iterator::advance(){
	if(get_depth()==0)
		return false;

	bool verbose = false;

	if(verbose) cout << "Advance: " << (void*) this << " from " << top();
	while(++(*this) && (top().first != top_state().begin()));
	
	if(verbose) {
		if(get_depth()>0)
			cout << " to " << top() << ".\n";
		else
			cout << " to END.\n";
	}

	return (get_depth()>0);
}

bool df_iterator::step_over(){
	bool at_end = false;
	int old_depth = get_depth();
	do{
		at_end = !(*this)++;
	}
	while (get_depth()>old_depth);

	return !at_end;
}

bool operator<=(const df_iterator& it1, const df_iterator& it2){
	for(int i=0; i<it1.stack.size(); i++)
		if(it1.stack[i].first->first<it2.stack[i].first->first)
			return true;
		else if(it1.stack[i].first->first>it2.stack[i].first->first)
				return false;
	
    return true;
}

bool operator==(const df_iterator& it1, const df_iterator& it2){
	if(it1.stack.size()!=it2.stack.size())
		return false;

	for(int i=0; i<it1.stack.size(); i++)
		if(it1.stack[i].first->first!=it2.stack[i].first->first)
			return false;
	
	return true;
} 

ostream& operator<<(ostream& out, const df_iterator& it){
	out << "Stack depth " << it.get_depth() << ": ";
	for(int i=0; i<it.get_depth(); i++){
		trace frame = it.stack[i];

		state* curr_state = frame.second; 
		out << curr_state->get_key() << "(";

		if(frame.first==curr_state->end())
			out << "END";		
		else{
			in_t input = frame.first->first;
			out_t output = frame.first->second.output;
			out << input << ":" << output << " -> " << frame.first->second.state;
		}
			out << ") ";

	}
	out.flush();
	return out;
}