#include "reduce_josh.cpp"
#include "reduce_greedy.cpp"
#include "reduce_censi.cpp"
#include "emulation.cpp"
#include "poisson_fsm.cpp"
#include "reduce_exact.cpp"
#include <ctime>

#include <fstream>


vector<in_t> read_sequence(istream& in, int n);

int main(){
	string 	database = "alberto",
			indir = "../data/" + database + "/",
			outdir = indir + "/output/";

	// string infile = indir + database + ".fsm";
	// fsm hh(infile);

	// cout << "The original file: " << hh << endl;

	const int DEPTH = 10;

	poisson_tree f(1, 10, 10, DEPTH);

	// cout << "The first-first tree:\n\n" << f << endl;
	// string treefile0 = outdir + database + "0.dot";
	// save_dot(treefile0, f);

	// string treefile1 = outdir + database + "1.dot";
	// decision_tree g1(f, 6);
	// cout << "The first tree:\n\n" << g1 << endl;
	// save_dot(treefile1, g1);

	// decision_tree g2(g1, 6);

	// int emulation_depth = 10;
	// if(emulates(g2, g1, emulation_depth))
	// 	cout << "Emulates at depth " << emulation_depth << ".\n";
	// else
	// 	cout << "Fails to emulate at depth " << emulation_depth << ".\n";

	clock_t t1 = clock();
	cover_t ct1 = cover_exact(f);
	clock_t t2 = clock();

	cout << "Exact (" << ct1.size() << " cliques): " << ct1 << endl;
	cout << "Computed in "  << (t2 - t1) / (double)(CLOCKS_PER_SEC / 1000) << " ms\n";


	clock_t t3 = clock();
	cover_t ct2 = cover_greedy(f);
	clock_t t4 = clock();

	cout << "Greedy (" << ct2.size() << " cliques): " << ct2 << ".\n";
	cout << "Computed in "  << (t4 - t3) / (double)(CLOCKS_PER_SEC / 1000) << " ms.\n";


	clock_t t5 = clock();
	cover_t ct3 = cover_josh(f);
	clock_t t6 = clock();

	cout << "Josh (" << ct3.size() << " cliques): " << ct3 << ".\n";
	cout << "Computed in "  << (t6 - t5) / (double)(CLOCKS_PER_SEC / 1000) << " ms.\n";


	clock_t t7 = clock();
	fsm g = reduce_censi(f, DEPTH);
	clock_t t8 = clock();

	cout << "Censi: " << g.get_size() << " states.\n";
	cout << "Computed in "  << (t8 - t7) / (double)(CLOCKS_PER_SEC / 1000) << " ms.\n";
	return 0;
}

vector<in_t> read_sequence(istream& in, int n){      
	vector<in_t> in_vec;
	in_t new_in;
	while(((n==-1) || (in_vec.size()<n)) && in>>new_in)
		in_vec.push_back(new_in);

	return in_vec;
}


