#include "decision_tree.h"

decision_tree::df_iterator decision_tree::begin(){
	decision_tree::df_iterator it(this);
	while(it.step_in());
	return it;
}

decision_tree::df_iterator decision_tree::end(){
	decision_tree::df_iterator it(this);	
	return it;
}

decision_tree::df_iterator::df_iterator(decision_tree* new_base){
	base = new_base;
	root = &(base->find_state(base->initial_state));
}

bool decision_tree::df_iterator::step_in(){	
	state* s = top();
	io_map_t::const_iterator it = s->begin();
	if(it==s->end())
		return false;

	return step_in(it->first);
}

bool decision_tree::df_iterator::step_in(in_t in){	
	state* s = top();
	io_map_t::const_iterator it = s->find(in);
	if(it==io_map.end())
		return false;	
	
	state* next = &(base->find_state(op.state));
	stack.push_back(trace(it, next));
	return true;
}

int decision_tree::df_iterator::get_depth(){
	return stack.size();
}

state* decision_tree::df_iterator::get_top() const{
	if(stack.size==0)
		return root;

	trace t = stack.peek();
	return t.second;
}

void decision_tree::df_iterator::operator++(){
	while(stack.size()>0){
		io_map_t::iterator& it = stack.pop_back().first;
		it++;
		if(it!=top()->io_map.end())
			break;
	}

	while(step_in());
}

friend bool operator<=(const decision_tree::df_iterator& it1, 
		decision_tree::const df_iterator& it2){
	for(int i=0; i<it1.stack.size; i++)
		if(it1[i]<it2[i])
			return true;
		else if(it1[i]>it2[i])
				return false;
	
    return true;
}


friend bool operator==(const decision_tree::df_iterator& it1, 
		decision_tree::const df_iterator& it2){
	if(it1.stack.size()!=it2.stack.size())
		return false;

	for(int i=0; i<it1.stack.size(); i++)
		if(it1.stack[i]!=it2.stack[i])
			return false;
	
	return true;
} 

