#ifndef DECISION_TREE_H
#define DECISION_TREE_H
#include "fsm.h"

/* DECISION TREE CLASS */
class decision_tree: public fsm{	
public:
	class df_iterator;
	
	void write_tree(ostream& out, int depth);
	void read_tree(istream& in, int depth);	
};



typedef pair<io_map_t::const_iterator, state*> trace;

/* DEPTH-FIRST ITERATOR CLASS */
class decision_tree::df_iterator{
private:	
	int max_depth;
	decision_tree* base;
	state* root, curr;

	vector<trace> stack;
	bool step_in();
	bool step_in(in_t in);
public:
	df_iterator(decision_tree* new_base, int new_depth);

	state* intersect(const df_iterator& right);

	trace& top();

	int get_depth() const;

	bool operator++();
	bool operator++(int k);

	friend bool operator<=(const df_iterator& it1, const df_iterator& it2);
	friend bool operator==(const df_iterator& it1, const df_iterator& it2);

	friend ostream& operator<<(ostream& out, const df_iterator& it);

	friend class decision_tree;
};

#endif // DECISION_TREE_H