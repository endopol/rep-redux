#ifndef DECISION_TREE_H
#define DECISION_TREE_H
#include "fsm.h"

class decision_tree: public fsm{	
protected:
	int depth;

public:
	class df_iterator;

	df_iterator begin();
	df_iterator end();
};

typedef pair<io_map_t::const_iterator, state*> trace;

class decision_tree::df_iterator{
private:	
	decision_tree* base;
	state* root;

	vector<trace> stack;
	bool step_in();
	bool step_in(in_t in);
public:
	df_iterator(decision_tree* new_base);

	state* top() const;

	int get_depth() const;

	void operator++();

	friend bool operator<=(const df_iterator& it1, const df_iterator& it2);
	friend bool operator==(const decision_tree::df_iterator& it1, 
		const decision_tree::df_iterator& it2);

	friend class decision_tree;
};



#endif // DECISION_TREE_H


