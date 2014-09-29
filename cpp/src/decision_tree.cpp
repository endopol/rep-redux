#include "decision_tree.h"

void decision_tree::write_tree(ostream& out, int depth){
	df_iterator it(this, depth);

	int new_depth, old_depth = 1; 
	
	bool finished = false;
	out << "{ ";
	while(!finished){
		state* curr_state = it.top().second;
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
	initial_state = old_state->getKey();
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

				old_state = it.top().second;

				state* new_state = &(add_state());

				int breakpoint = newtoken.find(':');

				istringstream is (newtoken.substr(0,breakpoint));
				istringstream os (newtoken.substr(breakpoint+1));

				in_t new_input;
				out_t new_output;

				is >> new_input;
				os >> new_output;

				bool success = old_state->add_io_map(new_input, outpair(new_output, new_state->getKey()));

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
			it.stack.pop_back();		

		//cout << it << endl;
	}
}

decision_tree::df_iterator::df_iterator(decision_tree* new_base, int new_depth){
	base = new_base;
	max_depth = new_depth;
	root = &(base->find_state(base->initial_state));

	stack.push_back(trace(root->begin(), root));
}

bool decision_tree::df_iterator::step_in(){	
	state* s = top().second;
	io_map_t::const_iterator it = top().first;
	if(it==s->end())
		return false;

	return step_in(it->first);
}

bool decision_tree::df_iterator::step_in(in_t in){	
	trace& top_trace = top();
	state* s = top_trace.second;
	io_map_t::const_iterator it = s->find(in);
	if(it==s->end())
		return false;
	
	skey_t next_key = it->second.state;
	state* next = &(base->find_state(next_key));
	stack.push_back(trace(next->begin(), next));
	return true;
}

int decision_tree::df_iterator::get_depth() const{
	return stack.size();
}

trace& decision_tree::df_iterator::top(){
	if(stack.size()==0)
		return (* (trace*) NULL);
	
	return stack.back();
}

state* decision_tree::df_iterator::intersect(const df_iterator& right){
	int max_depth = min(get_depth(), right.get_depth());

	state* common = NULL;
	for(int i=0; i<max_depth; i++)
		if(stack[i].first == right.stack[i].first)
			common = stack[i].second;
		else
			return common;
}

bool decision_tree::df_iterator::operator++(){
	if(get_depth()<max_depth && step_in()){				
		//cout << "Stepped into " << top().second->getKey() << ":" << top().first->first << ".\n";
		return true;
	}
	
	while(get_depth()>0 && (++top().first) == top().second->end()){

		stack.pop_back();
		if(&top()==NULL)
			return false;
	}
	//cout << "Stepped onto " << top().second->getKey() << ":" << top().first->first << "\n";
	return true;
}

bool decision_tree::df_iterator::operator++(int x){ ++(*this); }

bool operator<=(const decision_tree::df_iterator& it1, const decision_tree::df_iterator& it2){
	for(int i=0; i<it1.stack.size(); i++)
		if(it1.stack[i].first->first<it2.stack[i].first->first)
			return true;
		else if(it1.stack[i].first->first>it2.stack[i].first->first)
				return false;
	
    return true;
}


bool operator==(const decision_tree::df_iterator& it1, const decision_tree::df_iterator& it2){
	if(it1.stack.size()!=it2.stack.size())
		return false;

	for(int i=0; i<it1.stack.size(); i++)
		if(it1.stack[i].first->first!=it2.stack[i].first->first)
			return false;
	
	return true;
} 

ostream& operator<<(ostream& out, const decision_tree::df_iterator& it){
	out << "Stack depth " << it.get_depth() << ": ";
	for(int i=0; i<it.get_depth(); i++){
		trace frame = it.stack[i];

		state* curr_state = frame.second; 
		out << curr_state->getKey() << "(";

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