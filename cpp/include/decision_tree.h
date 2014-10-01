#ifndef DECISION_TREE_H
#define DECISION_TREE_H
#include "fsm.h"

/* DECISION TREE CLASS */
class decision_tree: public fsm{

public:	
	decision_tree(fsm in, int depth);

	void write_tree(ostream& out, int depth);
	void read_tree(istream& in, int depth);	
};

typedef pair<io_map_t::const_iterator, state*> trace;
ostream& operator<<(ostream& out, const trace& right);

/* DEPTH-FIRST ITERATOR CLASS */
class df_iterator{
private:	
	int max_depth;
	fsm* base;
	state* root, curr;

	vector<trace> stack;
	bool step_in(in_t in);
public:
	bool step_in();
	void step_out();
	df_iterator(fsm* new_base, int new_depth);
	df_iterator(fsm* new_base, state* first_state, int new_depth);

	trace& top();
	state& top_state();
	io_map_t::const_iterator& last_iterator();

	int get_depth() const;

	bool operator++();
	bool operator++(int k);
	bool advance();
	bool step_over();

	friend bool operator<=(const df_iterator& it1, const df_iterator& it2);
	friend bool operator==(const df_iterator& it1, const df_iterator& it2);

	friend ostream& operator<<(ostream& out, const df_iterator& it);

	friend class decision_tree;
};

#endif // DECISION_TREE_H