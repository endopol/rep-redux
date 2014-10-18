/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Top-level routines for evaluating the fsm reduction routines
 *
 *        Version:  1.0
 *        Created:  10/17/2014 12:52:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Joshua Hernandez (jah), endopol@gmail.com
 *   Organization:  UCLA Computer Vision Lab
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "reduce_josh.cpp"
#include "reduce_greedy.cpp"
#include "reduce_censi.cpp"
#include "emulation.cpp"
#include "poisson_fsm.cpp"
#include "reduce_exact.cpp"
#include "reduce_alberto.cpp"
#include "pathological_tree.cpp"
#include <iomanip>
#include <ctime>

#include <fstream>

vector<in_t> read_sequence(istream& in, int n);

int main(){


    /* Miscellaneous file i/o */
	// string 	database = "alberto",
	// 		indir = "../data/" + database + "/",
	// 		outdir = indir + "/output/";
    //
	// string infile = indir + database + ".fsm",
    //        outfile = outdir + database + ".dot";
	// fsm f(infile);
    // save_dot(outfile, f);         
	// cout << "The original file: " << f << endl;
	// return 0;
    //

    // Miscellaneous reduction examples
//    fsm f("../stats/10_17_2014/dump/dump-0040.fsm");
//    
//
//    cout << "Original:\n" << f << endl;
//    int lo_bound;
//    fsm fc = reduce_censi(f, 20);
//
//    cover_t cta = cover_alberto(f, lo_bound);
//    fsm fa = reduce(f, cta);
//
//   	cover_t cte = cover_exact(f, lo_bound, LONG_MAX);
//    fsm fe = reduce(f, cte);
//
//    cover_t ctj = cover_josh(f);
//    fsm fj = reduce(f, ctj);
//
//    save_dot("../data/demo/output/censi.dot",   fc);
//    save_dot("../data/demo/output/alberto.dot", fa);
//    save_dot("../data/demo/output/exact.dot",   fe);
//    save_dot("../data/demo/output/josh.dot",    fj);
//    
//    return 0;

	// const int NUM_REPEATS = 1000, MAX_D = 100, DEPTH_STEP = 5;
	//const string NAME_FORMAT = "../stats/patho_data-%02d.txt";
	const int NUM_REPEATS = 1000, MAX_D = 12, DEPTH_STEP = 2, MIN_STATES = 0, MAX_STATES = 700;
	const string NAME_FORMAT = "../stats/data-%02d.txt";

	long MAX_TIME = 10000000;

	int bad_count = 0;

	for(int depth = DEPTH_STEP; depth<=MAX_D; depth+=DEPTH_STEP){

		clock_t ta = clock();
		cout << "Depth " << depth << ": ";

		char outfile[50];
		sprintf(outfile, NAME_FORMAT.c_str(), depth);
		ofstream out(outfile);
        
        /* Stat file headers */
		out << "N_STATES  _DENSITY  LO_BOUND    _N_EXACT  _T_EXACT    _N_CENSI  _T_CENSI    "
            << "N_GREEDY  T_GREEDY    __N_JOSH  __T_JOSH    N_ALBRTO  T_ALBRTO\n";

		int decile = -1;
		for(int rep = 0; rep<NUM_REPEATS; rep++){

            /* Progress updates */
			while(decile<(rep*10)/NUM_REPEATS){
				decile++;
				cout << decile; 
				cout.flush();
			}

			// pathological_tree f(depth);
			poisson_tree f(1, 10, 20, depth, MIN_STATES, MAX_STATES);

			compat_t compat = compute_compat(f);

			out << setw(8) << f.get_size() << "  " << setw(8) << density(compat) << "  ";

            /* #####   EXACT   ################################################################## */
            clock_t t1 = clock();
			int lo_bound;
			cover_t ct1 = cover_exact(f, lo_bound, MAX_TIME);
			clock_t t2 = clock();
			out << setw(8) << lo_bound << "    ";
			if(lo_bound==ct1.size()){
				out << setw(8) << ct1.size() << "  " << setw(8) << (t2 - t1) / (double)(CLOCKS_PER_SEC / 1000) << "    ";
			}
			else{
				bad_count++;
				out << setw(8) << ct1.size() << "  " << 0 << "    ";	
				char dumpfile[50];
				sprintf(dumpfile, "../stats/dump-%04d.fsm", bad_count);
				ofstream dump(dumpfile);
				dump << f;
				dump.close();

				sprintf(dumpfile, "../stats/dump_stats-%04d.txt", bad_count);
				ofstream dump_stats(dumpfile);
				dump_stats << "N_STATES:\t" << f.get_size() << endl;
				dump_stats << "LO_BOUND:\t" << lo_bound << endl;
				dump_stats << "HI_BOUND:\t" << ct1.size() << endl;
				dump_stats << "CL_COVER:\t" << ct1 << endl;
				dump_stats.close();
			}
			// cout << "Exact: " << ct1.get_size() << " states.\n";
			// cout << "Exact cover computed in "  << (t2 - t1) / (double)(CLOCKS_PER_SEC / 1000) << " ms:\n";


            /* #####   CENSI   ################################################################## */		
			clock_t t7 = clock();
			fsm g = reduce_censi(f, depth);
			clock_t t8 = clock();
			if(!emulates(g,f,depth)){
				cout << "\n\n";
				cout << "Orig: " << f << endl;
				cout << "Censi: " << g << endl;

				save_dot("../data/censi/output/orig.dot", f);
				save_dot("../data/censi/output/cens.dot", g);
				return 0;
			}

			out << setw(8) << g.get_size() << "  " << setw(8) << (t8 - t7) / (double)(CLOCKS_PER_SEC / 1000) << "    ";
		    	
            /* #####   GREEDY   ################################################################# */
			clock_t t3 = clock();
			cover_t ct2 = cover_greedy(f);
			clock_t t4 = clock();

			out << setw(8) << ct2.size() << "  " << setw(8) << (t4 - t3) / (double)(CLOCKS_PER_SEC / 1000) << "    ";
            
            /* #####   JOSH    ################################################################## */
			clock_t t5 = clock();
			cover_t ct3 = cover_josh(f);
			clock_t t6 = clock();

			out << setw(8) << ct3.size() << "  " << setw(8) << (t6 - t5) / (double)(CLOCKS_PER_SEC / 1000) << "    ";

            /* #####   ALBERTO   ################################################################ */
			clock_t t9 = clock();
			cover_t ct4 = cover_alberto(f, lo_bound);
			clock_t t10 = clock();
			
			out << setw(8) << ct4.size() << "  " << setw(8) << (t10 - t9) / (double)(CLOCKS_PER_SEC / 1000) << "    ";
			if(!verify_cover(compat, ct4)){
                cout << "Compatibility matrix:\n" << compat << endl; 
				cout << "Bad cover: " << ct4 << endl;
				return 0;
			}
			out << endl;
		}
		out.close();
		clock_t tb = clock();
		cout << " - computed in "  << (tb - ta) / (double)(CLOCKS_PER_SEC / 1000) << " ms.  Saving to " << outfile << ". \n";
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


