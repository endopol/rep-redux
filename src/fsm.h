#ifndef FSM_H
#define FSM_H

#include <map>
#include <set>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

typedef char in_t;
typedef string key_t;
typedef int out_t;
const out_t UNDEFINED = -1;

typedef struct outpair{
	out_t output;
	key_t state;

	outpair() {}
	outpair(out_t new_output, key_t new_state) 
		: output(new_output), state(new_state) {}
} outpair;

typedef map<in_t, outpair> io_map_t;


/* STATE CLASS */
class fsm;
class state{
private:
	key_t key;
	io_map_t io_map;
	fsm* base;

	bool test_map(io_map_t new_io_map) const;
	bool test_map(in_t new_in, outpair new_outpair) const;
	bool test_map(in_t new_in, out_t new_out) const;

public:
	state(fsm* new_base, key_t new_key);
	state(fsm* new_base, key_t new_key, io_map_t new_io_map);	

	bool add_io_map(io_map_t new_io_map);
	bool add_io_map(in_t new_in, outpair new_outpair);
	bool add_io_map(in_t new_in, out_t new_out);

	outpair map(in_t input);

	friend class fsm;
	friend ostream& operator<< (ostream& out, const state& right);
	friend bool compatible(const state& s1, const state& s2);

};


/* FSM CLASS */
typedef map<key_t, state> state_map_t;
class fsm{
private:	
	state_map_t state_map;

	key_t initial_state;

	state* active_state;

	map<key_t, set<key_t> > compatibility;

	key_t get_new_key() const;
	void compute_compatibility();
public:
	state* add_state();
	state* add_state(key_t new_key);
	state* find_state(key_t query_key);
	
	void reset();
	outpair operate(in_t input, ostream& out);
	vector<outpair> operate(const vector<in_t>& in_vec, ostream& out);
	key_t get_active_state() const;

	bool compatible(key_t k1, key_t k2);
	void combine(key_t k1, key_t k2);

	friend ostream& operator<< (ostream& out, const fsm& right);
	friend istream& operator>> (istream& in, fsm& right);	
};

#endif
