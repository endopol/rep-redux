/*
 * =====================================================================================
 *
 *       Filename:  pathological_tree.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/17/2014 12:52:27 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Joshua Hernandez (jah), endopol@gmail.com
 *   Organization:  UCLA Computer Vision Lab
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "decision_tree.h"
#include <random>

class pathological_tree: public decision_tree{

public:
	pathological_tree(int tree_size){

		state* new_state = &(add_state());
		state* old_state = new_state;
		initial_state = new_state->get_key();

		if(tree_size==1)
			return;

		initial_state = new_state->get_key();

		vector<pair<in_t, out_t>> map1, map2;

		for(int i=0; i<tree_size; i++){
			map1.push_back(pair<in_t, out_t>((in_t) i+'a', (out_t) 1));
			map2.push_back(pair<in_t, out_t>((in_t) i+'a', (out_t) 2));
		}

		bool any_matches;
		do{
			any_matches = false;
			random_shuffle(map1.begin(), map1.end());
			random_shuffle(map2.begin(), map2.end());

			// cout << "Random permutation:\n";
			// for(int i=0; i<map1.size(); i++)
			// 	cout << map1[i].first << ":" << map1[i].second << " ";
			// cout << endl;

			// for(int i=0; i<map2.size(); i++)
			// 	cout << map2[i].first << ":" << map2[i].second << " ";
			// cout << "\n\n";

			for(int i=0; i<map1.size(); i++){
				pair<in_t, out_t> m1 = map1[i], m2 = map2[i];
				if(m1.first==m2.first){
					any_matches = true;
					break;
				}
			}
		}while(any_matches);


		for(int i=0; i<map1.size(); i++){
			state* new_state = &(add_state());
			skey_t new_key = new_state->get_key();

			pair<in_t, out_t> m1 = map1[i], m2 = map2[i];
		
			old_state->add_io_map((in_t) tree_size+i + 'a', outpair((out_t) 0, new_key));

			new_state->add_io_map(m1.first, outpair(m1.second, new_key));
			new_state->add_io_map(m2.first, outpair(m2.second, new_key));
		}

		decision_tree temp((fsm)*this, 2);

		// cout << temp << endl;

		*this = *((pathological_tree*) &temp);
	}
};
