#ifndef DECISION_TREE_H
#define DECISION_TREE_H
#include "fsm.h"

class decision_tree: public fsm{	
private:
	int depth;

public:
	class df_iterator;

	df_iterator begin();
	df_iterator end();
}

class decision_tree::df_iterator{
private:	
	decision_tree* base;

	vector<io_map_t::iterator> stack;
	bool step_in(in_t in);
public:
	df_iterator(decision_tree* new_base);

	int get_depth();

	bool operator++();

	friend bool operator<=(const df_iterator& it1, const df_iterator& it2);

	friend class decision_tree;
}



#endif // DECISION_TREE_H


