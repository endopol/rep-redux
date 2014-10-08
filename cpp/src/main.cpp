#include "reduce_josh.cpp"
#include "reduce_greedy.cpp"
#include "reduce_censi.cpp"
#include "emulation.cpp"
#include "poisson_fsm.cpp"
#include "reduce_exact.cpp"
#include "reduce_alberto.cpp"
#include <iomanip>
#include <ctime>

#include <fstream>


vector<in_t> read_sequence(istream& in, int n);

int main(){
	// string 	database = "alberto",
	// 		indir = "../data/" + database + "/",
	// 		outdir = indir + "/output/";

	// string infile = indir + database + ".fsm";
	// fsm hh(infile);

	// cout << "The original file: " << hh << endl;


	const int MIN_STATES = 0, MAX_STATES = 700,	NUM_REPEATS = 500, MAX_D = 50, DEPTH_STEP = 2;

	for(int depth = DEPTH_STEP; depth<=MAX_D; depth+=DEPTH_STEP){

		clock_t ta = clock();
		cout << "Depth " << depth << ": ";

		char outfile[20];
		sprintf(outfile, "data-%02d.txt", depth);		
		ofstream out(outfile);

		out << "N_STATES    _N_CENSI  _T_CENSI    N_GREEDY  T_GREEDY    __N_JOSH  __T_JOSH    LO_BOUND  N_ALBRTO  T_ALBRTO\n";

		for(int rep = 0; rep<NUM_REPEATS; rep++){

			poisson_tree f(1, 10, 20, depth, MIN_STATES, 100000);

			out << setw(8) << f.get_size() << "    ";

			// clock_t t1 = clock();
			// cover_t ct1 = cover_exact(f);
			// clock_t t2 = clock();


			// CENSI
			clock_t t7 = clock();
			fsm g = reduce_censi(f, depth);
			clock_t t8 = clock();

			out << setw(8) << g.get_size() << "  " << setw(8) << (t8 - t7) / (double)(CLOCKS_PER_SEC / 1000) << "    ";
			// cout << "Censi: " << g.get_size() << " states.\n";
			// cout << "Computed in "  << (t8 - t7) / (double)(CLOCKS_PER_SEC / 1000) << " ms.\n\n";


			// GREEDY
			clock_t t3 = clock();
			cover_t ct2 = cover_greedy(f);
			clock_t t4 = clock();

			out << setw(8) << ct2.size() << "  " << setw(8) << (t4 - t3) / (double)(CLOCKS_PER_SEC / 1000) << "    ";
			// cout << "Greedy (" << ct2.size() << " cliques): " << ct2 << ".\n";
			// cout << "Computed in "  << (t4 - t3) / (double)(CLOCKS_PER_SEC / 1000) << " ms.\n\n";


			// JOSH
			clock_t t5 = clock();
			cover_t ct3 = cover_josh(f);
			clock_t t6 = clock();

			out << setw(8) << ct3.size() << "  " << setw(8) << (t6 - t5) / (double)(CLOCKS_PER_SEC / 1000) << "    ";
			// cout << "Josh (" << ct3.size() << " cliques): " << ct3 << ".\n";
			// cout << "Computed in "  << (t6 - t5) / (double)(CLOCKS_PER_SEC / 1000) << " ms.\n\n";

			if(f.get_size()>MAX_STATES)
				continue;

			// ALBERTO
			int lo_bound;
			clock_t t9 = clock();
			cover_t ct4 = cover_alberto(f, lo_bound);
			clock_t t10 = clock();

			out << setw(8) << lo_bound << "  ";
			out << setw(8) << ct4.size() << "  " << setw(8) << (t10 - t9) / (double)(CLOCKS_PER_SEC / 1000) << "    ";
			// cout << "Alberto (" << ct4.size() << " cliques): " << ct4 << ".\n";
			// cout << "Computed in "  << (t10 - t9) / (double)(CLOCKS_PER_SEC / 1000) << " ms.\n\n";

			out << endl;
		}
		out.close();
		clock_t tb = clock();
		cout << "computed in "  << (tb - ta) / (double)(CLOCKS_PER_SEC / 1000) << " ms.\n";
	}
	return 0;
}

vector<in_t> read_sequence(istream& in, int n){      
	vector<in_t> in_vec;
	in_t new_in;
	while(((n==-1) || (in_vec.size()<n)) && in>>new_in)
		in_vec.push_back(new_in);

	return in_vec;
}


