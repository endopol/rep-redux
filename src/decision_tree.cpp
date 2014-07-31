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
	if(it==s->end())
		return false;	
	
	key_t next_key = it->second.state;
	state* next = &(base->find_state(next_key));
	stack.push_back(trace(it, next));
	return true;
}

int decision_tree::df_iterator::get_depth() const{
	return stack.size();
}

state* decision_tree::df_iterator::top() const{
	if(stack.size()==0)
		return root;

	trace t = stack.back();
	return t.second;
}

void decision_tree::df_iterator::operator++(){
	while(stack.size()>0){
		io_map_t::const_iterator& it = stack.back().first;
		stack.pop_back();
		it++;
		if(it!=top()->end())
			break;
	}

	while(step_in());
}

bool operator<=(const decision_tree::df_iterator& it1,
		const decision_tree::df_iterator& it2){
	for(int i=0; i<it1.stack.size(); i++)
		if(it1.stack[i].first->first<it2.stack[i].first->first)
			return true;
		else if(it1.stack[i].first->first>it2.stack[i].first->first)
				return false;
	
    return true;
}


bool operator==(const decision_tree::df_iterator& it1, 
		const decision_tree::df_iterator& it2){
	if(it1.stack.size()!=it2.stack.size())
		return false;

	for(int i=0; i<it1.stack.size(); i++)
		if(it1.stack[i].first->first!=it2.stack[i].first->first)
			return false;
	
	return true;
} 

