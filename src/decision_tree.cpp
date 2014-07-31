#include "decision_tree.h"
using namespace decision_tree;

df_iterator begin(){
	df_iterator it;
	while(it.step_in());
	return it;
}

df_iterator end(){
	df_iterator it(this);	
	return it;
}

df_iterator::df_iterator(decision_tree* new_base){
	base = new_base;
}

bool df_iterator::step_in(){
	state* top = stack.peek();
	const io_map_t& io_map = top.first->io_map;
	io_map_t::const_iterator it = io_map.begin();
	if(it==io_map.end())
		return false;

	return step_in(it->first);
}

bool df_iterator::step_in(in_t in){
	state* top = stack.peek();
	outpair op = (*top)(in);
	if(op.output==UNDEFINED)
		return false;
	
	state* next = &(base->find_state(op.state));
	stack.push_back(next);
	return true;
}

int df_iterator::get_depth(){
	return stack.size();
}


void df_iterator::operator++(){

}

friend bool operator<=(const df_iterator& it1, const df_iterator& it2){
	for(int i=0; i<it1.stack.size; i++)
		if(it1[i]<it2[i])
			return true;
		else if(it1[i]>it2[i])
				return false;
	
    return true;
}


friend bool operator==(const df_iterator& it1, const df_iterator& it2){
	if(it1.stack.size()!=it2.stack.size())
		return false;

	for(int i=0; i<it1.stack.size(); i++)
		if(it1.stack[i]!=it2.stack[i])
			return false;
	
	return true;
} 
