/*
 * =====================================================================================
 *
 *       Filename:  fsm.h
 *
 *    Description:  Classes and functions for manipulation of Finite State Machines.
 *
 *        Version:  1.0
 *        Created:  10/17/2014 12:13:55 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Joshua Hernandez (jah), endopol@gmail.com
 *   Organization:  UCLA Computer Vision Lab
 *
 * =====================================================================================
 */

#ifndef FSM_H
#define FSM_H

#include <map>
#include <set>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

/* #####   EXPORTED TYPE DEFINITIONS   ############################################## */

/* Input/output types */
typedef char in_t;
typedef int out_t;
const out_t UNDEFINED = -1;

/*
 * =====================================================================================
 *        Class:  skey_t
 *  Description:  Comparable state key class, serving as unique identifiers for states.
 * =====================================================================================
 */

class skey_t{
	int keynum;
public:
	skey_t();
	skey_t(int new_keynum);

	bool operator<(const skey_t& right) const;
	bool operator==(const skey_t& right) const;

	friend istream& operator>>(istream& in, skey_t& right);	
	friend ostream& operator<<(ostream& out, const skey_t& right);	
};

/* Output (control, state) pair */
typedef struct outpair{
	out_t output;
	skey_t state;

	outpair() {}
	outpair(out_t new_output, skey_t new_state) 
		: output(new_output), state(new_state) {}
} outpair;


/*
 * =====================================================================================
 *        Class:  state
 *  Description:  Implements a general-purpose input/output function with transfer 
 *  functions to other states (known to this state by their keys).  Mappings are stored
 *  in the "io_map" field.
 * =====================================================================================
 */

/* Input/output map */
typedef map<in_t, outpair> io_map_t;

class fsm;
class state{		

protected:
	skey_t key;
	io_map_t io_map;
	bool test_io_map(in_t new_in, outpair new_outpair) const;	

public:
	state(){};
	state(skey_t new_key);
	state(skey_t new_key, io_map_t new_io_map);	

    /* Comparison/combination functions */
	bool test_io_map(const state& s) const;
	bool add_io_map(const state& s);
	bool add_io_map(in_t new_in, outpair new_outpair);
	bool add_io_map(in_t new_in, outpair new_outpair, bool replace);

    /* Iterator routines */

	friend ostream& operator<<(ostream& out, const state& right);

    /* Accessor methods */
    outpair operator()(in_t input) const;
	virtual io_map_t::const_iterator find(in_t input) const;
	io_map_t::const_iterator begin() const;
	io_map_t::const_iterator end() const;

    skey_t get_key() const;
    int get_size() const;
    io_map_t& get_io_map();
        

	friend class fsm;
};


/*
 * =====================================================================================
 *        Class:  fsm
 *  Description:  Finite State Machine - a graph of states, with edges defined by their
 *  transfer functions.  States are stored in the "state_map" field.
 * =====================================================================================
 */

/* Key-to-state map */
typedef map<skey_t, state> state_map_t;

class fsm{

protected:	
	state_map_t state_map;

	skey_t initial_state;

	state* active_state;

	bool test_compat(skey_t k1, skey_t k2) const;
	bool test_compat(const state& s1, const state& s2) const;

public:
	fsm(){}
	fsm(string filename);

    /* State Manipulation */
	state& add_state();
	state& add_state(skey_t new_key);
	state& find_state(skey_t query_key);

	/* Public operating interface */
	void reset();
	outpair operate(in_t input, ostream& out);
	vector<outpair> operate(const vector<in_t>& in_vec, ostream& out);
	skey_t get_active_state() const;
	skey_t get_initial_state() const;
	void set_initial_state(skey_t new_initial_state);

    /* Accessors */
	state_map_t& get_state_map();
	int get_size() const;

    /* Read/Write routines */
   	void save_dot(ostream& out) const;
	friend void save_dot(string filename, const fsm& right);
	friend ostream& operator<<(ostream& out, const fsm& right);
	friend istream& operator>>(istream& in, fsm& right);	

};


/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */
/*
 * =====================================================================================
 *         Type:  compat_t
 *  Description:  Map to sets of keys, representing the compatibility relations (ability
 *  to be combined without affecting performance) between states of an fsm.
 * =====================================================================================
 */
typedef map<skey_t, set<skey_t> > compat_t;

compat_t compute_compat(fsm& right);
void symmetrize(compat_t& compat);
float density(compat_t& compat);
ostream& operator<<(ostream& out, const compat_t& right);

/*
 * =====================================================================================
 *         Type:  cover_t
 *  Description:  Vector of sets of keys, representing a set of cliques (sets of nodes 
 *  which are all mutually compatible) on a given fsm.
 * =====================================================================================
 */
typedef vector<set<skey_t> > cover_t;

fsm reduce(fsm& orig, const cover_t& X);

bool add_to_clique(skey_t new_entry, set<skey_t>& new_clique, const compat_t& compat);
bool add_to_clique_symmetric(skey_t new_entry, set<skey_t>& new_clique, const compat_t& compat);
ostream& operator<<(ostream& out, const cover_t& right);


bool verify_cover(compat_t& ct, cover_t& x);

#endif
