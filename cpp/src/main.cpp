// #include "reduce_josh.cpp"
// #include "reduce_greedy.cpp"
#include "poisson_fsm.cpp"
#include "reduce_exact.cpp"

#include <fstream>


vector<in_t> read_sequence(istream& in, int n);

int main(){
	string 	database = "alberto",
			indir = "../data/" + database + "/",
			outdir = indir + "/output/";

	// string infile = indir + database + ".fsm";
	// fsm f(infile);

	poisson_tree f(2, 10, 10, 10);

	string treefile1 = outdir + database + "1.dot";
	decision_tree g1(f, 10);
	cout << "The first tree:\n\n" << g1 << endl;
	save_dot(treefile1, g1);

	// fsm g2 = reduce_josh_exhaustive(f);
	// cout << "The reduced tree:\n\n"  << g2 << endl;
	// // string treefile2 = outdir + database + "2.dot";
	// // save_dot(treefile2, decision_tree(reduce_josh(f),5));

	// int emulation_depth = 10;
	// if(emulates(g2, g1, emulation_depth))
	// 	cout << "Emulates at depth " << emulation_depth << ".\n";
	// else
	// 	cout << "Fails to emulate at depth " << emulation_depth << ".\n";

	//cover_t ct = cover_exact(f);

	//cout << "Cliques: " << ct << endl;

	return 0;
}

vector<in_t> read_sequence(istream& in, int n){      
	vector<in_t> in_vec;
	in_t new_in;
	while(((n==-1) || (in_vec.size()<n)) && in>>new_in)
		in_vec.push_back(new_in);

	return in_vec;
}


