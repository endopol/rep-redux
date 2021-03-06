/*
 * =====================================================================================
 *
 *       Filename:  decision_tree.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/17/2014 12:46:35 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Joshua Hernandez (jah), endopol@gmail.com
 *   Organization:  UCLA Computer Vision Lab
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "decision_tree.h"

/* #####   DECISION TREE METHODS   ################################################## */

/* DECISION TREE CONSTRUCTORS */
decision_tree::decision_tree(fsm in, int depth){
	state* new_state = &(add_state());
	initial_state = new_state->get_key();
	df_iterator tt(this, depth);

	df_iterator it(&in, depth);

	do{			
		while(tt.get_depth() > it.get_depth())
			tt.step_out();

		if(it.top_state().get_size()==0)
			continue;

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

/* I/O METHODS */
/*
 *--------------------------------------------------------------------------------------
 *       Class:  decision_tree
 *      Method:  decision_tree :: write_tree
 * Description:  Writes out a tree in nested-list format
 *   e.g. {a:1 { b:2, c:3 { c:5 } }, e:4 }.
 *--------------------------------------------------------------------------------------
 */
void decision_tree::write_tree(ostream& out, int depth){
	df_iterator it(this, depth);

	int new_depth, old_depth = 1; 
	
	bool finished = false;
	out << "{ ";
	while(!finished){
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

/*
 *--------------------------------------------------------------------------------------
 *       Class:  decision_tree
 *      Method:  decision_tree :: read_tree
 * Description:  Reads in a tree in nested-list format 
 *  e.g. {a:1 { b:2, c:3 { c:5 } }, e:4 }
 *--------------------------------------------------------------------------------------
 */
int count(const string& str, char c); // Count occurrences of a given character in a given string

void trim(string& s, const char* delim); // Trim leading and trailing instances of characters in given array

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

				old_state->add_io_map(new_input, outpair(new_output, new_state->get_key()));

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


/* #####   DF_ITERATOR METHODS   #################################################### */

/* DF_ITERATOR CONSTRUCTORS */

df_iterator::df_iterator(fsm* new_base, int new_depth){
	base = new_base;
	max_depth = new_depth;
	root = &(base->find_state(base->get_initial_state()));

	stack.push_back(trace_t(root->begin(), root));
}

df_iterator::df_iterator(fsm* new_base, state* first_state, int new_depth){
	base = new_base;
	max_depth = new_depth;
	root = first_state;

	stack.push_back(trace_t(root->begin(), root));	
}


/* LOW-LEVEL TRAVERSAL METHODS */
/*
 *--------------------------------------------------------------------------------------
 *       Class:  df_iterator
 *      Method:  df_iterator :: step_in 
 * Description:  Step into the first child
 *--------------------------------------------------------------------------------------
 */
bool df_iterator::step_in(){
	if(get_depth()==0)
		return false;

	state& s = top_state();
	io_map_t::const_iterator it = top().first;

	if(s.get_size()==0){
		stack.push_back(trace_t(s.end(), &s));
	}

	if(it==s.end()){
		return false;
	}

	return step_in(it->first);
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  df_iterator
 *      Method:  df_iterator :: step_in(skey_t)
 * Description:  Step into the given child
 *--------------------------------------------------------------------------------------
 */
bool df_iterator::step_in(in_t in){	
	// cout << "STEPPING IN TO " << in << endl;

	trace_t& top_trace = top();
	state* s = top_trace.second;
	io_map_t::const_iterator it = s->find(in);
	if(it==s->end())
		return false;
	
	skey_t next_key = it->second.state;
	state* next = &(base->find_state(next_key));
	stack.push_back(trace_t(next->begin(), next));
	return true;
}

void df_iterator::step_out(){
	stack.pop_back();
}


/* TRAVERSAL METHODS */
/*
 *--------------------------------------------------------------------------------------
 *       Class:  df_iterator
 *      Method:  df_iterator :: operator++
 * Description:  Traverse edges in depth-first order
 *--------------------------------------------------------------------------------------
 */
bool df_iterator::operator++(){
	// cout << "Started at " << top_state().get_key() 
	// 	 	<< " (" << top().first->first << ":" << top().first->second.output << ").\n";

	if(get_depth()<max_depth && step_in()){				
		// cout << "Stepped into " << top_state().get_key() 
		 	// << " (" << top().first->first << ":" << top().first->second.output << ").\n";
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

	// cout << "Stepped onto " << top_state().get_key()
	// 	<< " (" << top().first->first << ":" << top().first->second.output << ").\n";
	return true;
}

bool df_iterator::operator++(int x){ return ++(*this); }

/*
 *--------------------------------------------------------------------------------------
 *       Class:  df_iterator
 *      Method:  df_iterator :: advance
 * Description:  Traverse nodes in depth-first order
 *--------------------------------------------------------------------------------------
 */
bool df_iterator::advance(){
	if(get_depth()==0)
		return false;

	bool verbose = false;

	if(verbose) cout << "Advance: " << (void*) this << " from " << top();
	// TODO: This is likely where everything got fucked up!
	while(++(*this) && ((top().first != top_state().begin()) || (top().first == top_state().end())));
	
	if(verbose) {
		if(get_depth()>0)
			cout << " to " << top() << ".\n";
		else
			cout << " to END.\n";
	}

	return (get_depth()>0);
}


/* COMPARISON OF DF_OPERATORS */
/*
 *--------------------------------------------------------------------------------------
 *       Class:  df_iterator
 *      Method:  df_iterator :: breakpoint
 * Description:  Computes the depth of the deepest common ancestor of two iterators
 *--------------------------------------------------------------------------------------
 */
int breakpoint(df_iterator& i1, df_iterator& i2){
	vector<trace_t>& s1 = i1.stack;
	vector<trace_t>& s2 = i2.stack;

	int bp = 0;	
	while(s1[bp].second == s2[bp].second && bp < min(s1.size(), s2.size()))
		bp++;

	return bp;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  df_iterator
 *      Method:  df_iterator :: operator<=
 * Description:  Lexicographic comparisons of two iterators
 *--------------------------------------------------------------------------------------
 */
bool operator<=(const df_iterator& it1, const df_iterator& it2){
	for(int i=0; i<it1.stack.size(); i++)
		if(it1.stack[i].first->first<it2.stack[i].first->first)
			return true;
		else if(it1.stack[i].first->first>it2.stack[i].first->first)
				return false;
	
    return true;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  df_iterator
 *      Method:  df_iterator :: operator==
 * Description:  Equivalence testing of two iterators
 *--------------------------------------------------------------------------------------
 */
bool operator==(const df_iterator& it1, const df_iterator& it2){
	if(it1.stack.size()!=it2.stack.size())
		return false;

	for(int i=0; i<it1.stack.size(); i++)
		if(it1.stack[i].first->first!=it2.stack[i].first->first)
			return false;
	
	return true;
} 


/* ACCESSOR METHODS */
int df_iterator::get_depth() const{
	return stack.size();
}

trace_t& df_iterator::top(){
	if(stack.size()==0)
		return (* (trace_t*) NULL);
	
	return stack.back();
}

state& df_iterator::top_state(){
	trace_t& top_trace = top();
	if(&top_trace==NULL)
		return (* (state*) NULL);
	else return *(top_trace.second);
}


/* I/O ROUTINES */
ostream& operator<<(ostream& out, const df_iterator& it){
	out << "Stack depth " << it.get_depth() << ": ";
	for(int i=0; i<it.get_depth(); i++){
		trace_t frame = it.stack[i];

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

// stream operator for printing out iterator stack traces
ostream& operator<<(ostream& out, const trace_t& right){
	if(right.second!=NULL)
		out << right.second->get_key();
	else
		out << "NULL";

	out << "(" << right.first->first << ")";
	return out;
}

