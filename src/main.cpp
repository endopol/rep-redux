#include "fsm.h"
#include <fstream>


vector<in_t> read_sequence(istream& in, int n);

int main(){

	fsm f;

	string infile = "out.fsm";
	ifstream in(infile.c_str());
	in >> f;
	in.close();

	cout << "\nThe stored FSM:\n\n" << f << endl;

	string outfile = "out2.fsm";
	ofstream out(outfile.c_str());
	out << f;
	out.close();

	f.reset();
	string sequence = "a c a c d";
	stringstream ss(sequence);
	vector<in_t> v_in = read_sequence(ss, -1);
	f.operate(v_in, cout);
}

vector<in_t> read_sequence(istream& in, int n){
	vector<in_t> in_vec;
	in_t new_in;
	while(((n==-1) || (in_vec.size()<n)) && in>>new_in)
		in_vec.push_back(new_in);

	return in_vec;
}

