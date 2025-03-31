#include <Tools.hpp>
#include <chrono>
#include <random>
void Scale_free_network(boost::container::vector <bool>* matrix, uint64_t nodes){
	boost::container::vector <uint64_t> vet;
	std::default_random_engine gen;
    //std::uniform_real_distribution<double> uniform;

    std::uniform_real_distribution<double> uniform(0,1);
	gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
	for (uint64_t i = 0; i < nodes; i++)
		vet.push_back(i);


	uint64_t s = vet.size();
	while (s > 0){
		double dNotes = static_cast<double>(s);
		uint64_t num = static_cast<uint64_t>(uniform(gen) * s);
		uint64_t v = vet[num];
		vet[num] = vet[vet.size()-1];
		vet.pop_back();
		s = vet.size();
	}

}
