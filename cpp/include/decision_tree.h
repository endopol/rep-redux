/*
 * =====================================================================================
 *
 *       Filename:  decision_tree.h
 *
 *    Description:  Classes and routines for tree-structured FSMs
 *
 *        Version:  1.0
 *        Created:  10/17/2014 12:12:29 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Joshua Hernandez (jah), endopol@gmail.com
 *   Organization:  UCLA Computer Vision Lab
 *
 * =====================================================================================
 */

#ifndef DECISION_TREE_H
#define DECISION_TREE_H
#include "fsm.h"


/*
 * =====================================================================================
 *        Class:  decision_tree
 *  Description:  Class for handling tree-structured FSMs
 * =====================================================================================
 */
class decision_tree: public fsm{

public:	
	decision_tree(){}
	decision_tree(fsm in, int depth);

    /* Read/Write Treee in nested list notation */
	void write_tree(ostream& out, int depth);
	void read_tree(istream& in, int depth);	
};


/*
 * =====================================================================================
 *        Class:  df_iterator
 *  Description:  Depth-first iterator for traversing a decision tree
 * =====================================================================================
 */

typedef pair<io_map_t::const_iterator, state*> trace_t;
ostream& operator<<(ostream& out, const trace_t& right);

class df_iterator{
private:	
	int max_depth;      // Maximum allowed depth for traversal
	fsm* base;          // Associated fsm
	state* root;

   	vector<trace_t> stack;  // Traversal history of this iterator

	bool step_in(in_t in);  // Enter given child
public:
	df_iterator(fsm* new_base, int new_depth);
	df_iterator(fsm* new_base, state* first_state, int new_depth);

    /* Accessor routines */
	int get_depth() const;
	int get_max_depth() const { return max_depth; }
	fsm* get_base() { return base; }
	state* get_state(int i){ return stack[i].second; }

    trace_t& top();
	state& top_state();

    /* Traversal routines */
   	bool step_in();     // Enter first child
	void step_out();    // Enter parent
	bool operator++();      // Traverse edges, depth-first order
	bool operator++(int k); // k-at-a-time edge traversal
	bool advance();         // Traverse nodes in depth-first order

    /* Comparison of df_iterators */
	friend bool operator<=(const df_iterator& it1, const df_iterator& it2);
	friend bool operator==(const df_iterator& it1, const df_iterator& it2);
	friend int breakpoint(df_iterator& i1, df_iterator& i2);

  	friend ostream& operator<<(ostream& out, const df_iterator& it);

	friend class decision_tree;
};

#endif // DECISION_TREE_H
