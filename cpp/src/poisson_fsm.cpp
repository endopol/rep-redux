#include "decision_tree.h"
#include <random>
#include <chrono>

class poisson_tree: public decision_tree{

public:
	poisson_tree(double lambda, in_t num_inputs, out_t num_outputs, int max_height){
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		default_random_engine generator(seed);
    	poisson_distribution<int> distribution(lambda);

		state& root = add_state();
		initial_state = root.get_key();
		df_iterator it(this, max_height);
		do{
			it.step_in();

			cout << "Trace: " << it.top() << endl;

			int num_children = distribution(generator);
			cout << "num_children: " << num_children << endl;

			for(int i=0; i<num_children; i++){
				in_t in = rand()%num_inputs + 'a';
				out_t out = rand()%num_outputs;
				state& new_state = add_state();

				it.top_state().add_io_map(in, outpair(out, new_state.get_key()));
			}
		}while(it++);
	}
};
