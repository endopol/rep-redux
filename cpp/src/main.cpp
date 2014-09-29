#include "decision_tree.h"
#include <fstream>


vector<in_t> read_sequence(istream& in, int n);

int main(){

	decision_tree f;

	string 	database = "alberto",
			dir = "../data/" + database + "/";

	string infile = dir + database + ".fsm";
	ifstream in(infile.c_str());
	in >> f;
	in.close();

	cout << "\nThe stored FSM:\n\n" << f << endl;
	string fsm_dot_file = dir + database + ".dot";	
	ofstream fsm_dot_out(fsm_dot_file.c_str());
	f.save_dot(fsm_dot_out);
	fsm_dot_out.close();

	string tree_file = dir + database + ".tree";
	ofstream tree_out(tree_file.c_str());
	f.write_tree(tree_out, 3);
	tree_out.close();
	
	f = decision_tree();
	ifstream tree_in(tree_file.c_str());
	f.read_tree(tree_in, 4);
	tree_in.close();

	cout << "\nThe linearized FSM:\n\n" << f << endl;
	string tree_dot_file = dir + database + "_tree.dot";	
	ofstream tree_dot_out(tree_dot_file.c_str());
	f.save_dot(tree_dot_out);
	tree_dot_out.close();

	return 0;
}

vector<in_t> read_sequence(istream& in, int n){
	vector<in_t> in_vec;
	in_t new_in;
	while(((n==-1) || (in_vec.size()<n)) && in>>new_in)
		in_vec.push_back(new_in);

	return in_vec;
}

