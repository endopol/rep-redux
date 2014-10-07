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

/* INPUT/OUTPUT CLASSES */
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



typedef char in_t;
typedef int out_t;
const out_t UNDEFINED = -1;

typedef struct outpair{
	out_t output;
	skey_t state;

	outpair() {}
	outpair(out_t new_output, skey_t new_state) 
		: output(new_output), state(new_state) {}
} outpair;



/* STATE CLASS */
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

	bool test_io_map(const state& s) const;
	bool add_io_map(const state& s);
	bool add_io_map(in_t new_in, outpair new_outpair);
	bool add_io_map(in_t new_in, outpair new_outpair, bool replace);

	outpair operator()(in_t input) const;
	virtual io_map_t::const_iterator find(in_t input) const;
	io_map_t::const_iterator begin() const;
	io_map_t::const_iterator end() const;

	friend ostream& operator<<(ostream& out, const state& right);

	skey_t get_key() const {return key;}
	int get_size() const {return io_map.size();}
	io_map_t& get_io_map() {return io_map;}

	friend class fsm;
};


typedef vector<set<skey_t> > cover_t;
typedef map<skey_t, set<skey_t> > compat_t;
typedef map<skey_t, state> state_map_t;

/* FSM CLASS */
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

	state& add_state();
	state& add_state(skey_t new_key);
	state& find_state(skey_t query_key);

	// Public operating interface
	void reset();
	outpair operate(in_t input, ostream& out);
	vector<outpair> operate(const vector<in_t>& in_vec, ostream& out);
	skey_t get_active_state() const;
	skey_t get_initial_state() const;
	void set_initial_state(skey_t new_initial_state){ initial_state = new_initial_state; }

	void combine(skey_t k1, skey_t k2);

	friend ostream& operator<<(ostream& out, const fsm& right);
	friend istream& operator>>(istream& in, fsm& right);	

	friend compat_t compute_compat(fsm& right);

	state_map_t& get_state_map(){ return state_map; }
	int get_size() const { return state_map.size(); }

	void save_dot(ostream& out) const;
	friend void save_dot(string filename, const fsm& right);
};

ostream& operator<<(ostream& out, const compat_t& right);
ostream& operator<<(ostream& out, const cover_t& right);
fsm reduce(const fsm& orig, const cover_t& X);

#endif
