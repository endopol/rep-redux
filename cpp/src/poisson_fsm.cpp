#include "decision_tree.h"
#include <random>
#include <chrono>

class poisson_tree: public decision_tree{

public:
	poisson_tree(double lambda, in_t num_inputs, out_t num_outputs, int max_height, int min_states, int max_states){
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		default_random_engine generator(seed);
    	poisson_distribution<int> distribution(lambda);

		int depth = 0;

		while(depth<max_height || state_map.size()>max_states || state_map.size()<min_states){
			state_map = state_map_t();

			state& root = add_state();
			initial_state = root.get_key();
			df_iterator it(this, max_height);
						
			depth = 0;
			do{
				depth = max(depth, it.get_depth());

				int num_children = distribution(generator);
				// cout << "num_children: " << num_children << endl;

				// cout << "Trace: " << it << endl;

				state& curr_state = it.top_state();
				if(curr_state.get_size()>0)
					continue;

				for(int i=0; i<num_children; i++){
					in_t in = rand()%num_inputs + 'a';
					out_t out = rand()%num_outputs;
					if(curr_state.find(in)!=curr_state.end())
						continue;

					state& new_state = add_state();
					curr_state.add_io_map(in, outpair(out, new_state.get_key()));

					// cout << "map " << in << ":" << out << "->" << new_state.get_key() << endl;
				}

				//cout << it.top_state() << endl;
			}while(it++);

		}
		// cout << state_map.size() << endl;
	}
};
