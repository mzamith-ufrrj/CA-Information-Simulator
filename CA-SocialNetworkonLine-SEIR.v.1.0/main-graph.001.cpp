#include <fstream>
#include <chrono>
#include <random>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <boost/container/vector.hpp>
#include <boost/range/algorithm/find.hpp>
#include <iomanip>
using namespace boost::container;
using namespace std;
void Scale_free_network(boost::container::vector <bool>* matrix, uint64_t nodes, std::string filename);
void print(boost::container::vector <bool>* matrix, uint64_t nodes){
	cout << endl;
	for (uint64_t j = 0; j < nodes; j++){
		for (uint64_t i = 0; i < nodes; i++){
			uint64_t p = j * nodes + i;
			cout << matrix->begin()[p] << " ";
		}
		cout << endl;
	}

}
int main (int ac, char **av){
	uint64_t nodes = std::stoi(av[1]);
	std::string filename = "log-" + std::string(av[1]) + ".txt";
	std::cout << std::endl << std::endl << "Scale-free network - creating" << std::endl;
	std::cout << "Nodes: " << nodes << std::endl;
	std::cout << "  Log: " << filename << std::endl;
	boost::container::vector <bool> matrix;
	matrix.resize(nodes * nodes);
	for (uint64_t j = 0; j < nodes; j++){
		for (uint64_t i = 0; i < nodes; i++){
			uint64_t p = j * nodes + i;
			matrix[i] = false;
		}
	}
	//print(&matrix, nodes);
	Scale_free_network(&matrix, nodes, filename);
	return EXIT_SUCCESS;
}


void Scale_free_network(boost::container::vector <bool>* matrix, uint64_t nodes, std::string filename){
	boost::container::vector <uint64_t> vetK;
	boost::container::vector <double> vetP;
	boost::container::vector <uint64_t> vetS, blackList;


	std::default_random_engine gen;
	double kTotal = 0.0;
	uint64_t kMax = 0;
	uint64_t kMin = 2;
    //std::uniform_real_distribution<double> uniform;

    std::uniform_real_distribution<double> uniform(0,1);
	gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
	for (uint64_t i = 0; i < nodes; i++){
		vetK.push_back(0);
		vetP.push_back(0.0);
	}

	matrix->begin()[0 * nodes + 1] = true;
	matrix->begin()[1 * nodes + 0] = true;

	matrix->begin()[2 * nodes + 0] = true;
	matrix->begin()[2 * nodes + 1] = true;

	matrix->begin()[0 * nodes + 2] = true;
	matrix->begin()[1 * nodes + 2] = true;

	kTotal = 6.0;
	vetK[0] = 2.0;
	vetK[1] = 2.0;
	vetK[2] = 2.0;
	vetP[0] = vetK[0] / kTotal;
	vetP[1] = vetP[0] + (vetK[1] / kTotal);
	vetP[2] = vetP[1] + (vetK[2] / kTotal);

	//print(matrix, nodes);
	//std::cout << "=====================================================================================================================" << std::endl;
	for (uint64_t n = 0; n < nodes; n++)std::cout << std::setw(6) << vetK[n] << " ";
	std::cout << " \t\tK:" << kTotal << std::endl;
	for (uint64_t n = 0; n < nodes; n++) std::cout << std::setw(7) << std::fixed << std::setprecision(4) << vetP[n] ;

	std::cout << std::endl;


	for (uint64_t i = 2; i < nodes; i++){ //Quem estou inserindo;
		blackList.clear();
		blackList.push_back(i);
		uint64_t n = 0;
		while (n < kMin){
			double p = uniform(gen);
			double acc = vetP[0];
			uint64_t j = 1;
			while (p > acc)
				acc+=vetP[j++];
			assert(j >= 1.0);
			j--;
			auto ret = boost::find(blackList, j);
			if (ret == blackList.end()){//not found
				matrix->begin()[i * nodes + j] = true;
				matrix->begin()[j * nodes + i] = true;
				blackList.push_back(j);
				vetK[i]++;
				vetK[j]++;
				kTotal+=2;
				for (uint64_t m = 0; m <= i; m++){
					double a = 0.0;
					if (m >= 1) a = vetP[m-1];
					vetP[m] = a + (vetK[m] / kTotal);
				}//for (uint64_t m = 0; m <= i; m++){
				n++;
			}//if (ret == blackList.end()){//not found
		}//for (uint64_t n = 0; n < kmin; n++){

		//print(matrix, nodes);
		/*
		std::cout << "=====================================================================================================================" << std::endl;
		for (uint64_t n = 0; n < nodes; n++)std::cout << std::setw(6) << vetK[n] << " ";
		std::cout << " \t\tK:" << kTotal << std::endl;
		for (uint64_t n = 0; n < nodes; n++) std::cout << std::setw(7) << std::fixed << std::setprecision(4) << vetP[n] ;
		std::cout << " \t\t" << "p(" << p << ")" << std::endl;
		std::cout << std::endl;
		 */




	}//for (uint64_t i = 2; i < nodes; i++){ //Quem estou inserindo;

	print(matrix, nodes);

	std::cout << "=====================================================================================================================" << std::endl;
	for (uint64_t n = 0; n < nodes; n++){
		std::cout << std::setw(7) << vetK[n] << " ";
		uint64_t aux = static_cast<uint64_t>(vetK[n]);
		if (kMax < aux)
			kMax = aux;
	}


	std::cout << std::endl;
	for (uint64_t n = 0; n < nodes; n++) std::cout << std::setw(7) << std::fixed << std::setprecision(4) << vetP[n] << " ";
	std::cout << std::endl;
	std::cout << "Total de conexões: " << std::setw(7) << std::fixed << std::setprecision(1) << kTotal << " " << std::endl;
	std::cout << "            Média: " << std::setw(7) << std::fixed << std::setprecision(5) << kTotal / static_cast<double>(nodes) << " " << std::endl;
	std::cout << "           Máximo: " << std::setw(7) << std::fixed << std::setprecision(5) << kMax << " ";
	std::cout << std::endl << std::endl;

	vetS.resize(kMax);
	for (uint64_t n = 0; n < nodes; n++){
		uint64_t v = static_cast<uint64_t>(vetK[n]);
		vetS[v-1]++;
	}

	std::fstream output;
	output.open(filename, std::fstream::out|std::fstream::trunc);
	assert(output.is_open());
	for (uint64_t n = 0; n < vetS.size(); n++)
		output << n+1 << " " << vetS[n] << std::endl;
	output.close();
}



