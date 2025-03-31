#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <cstdlib>
#include <chrono>
int main(int ac, char **av){
	std::default_random_engine generator;
	std::normal_distribution<double> normal(1.0, 0.25);
    std::weibull_distribution<double> weibull(2.0,4.0);
    std::lognormal_distribution<double> lognormal(0.0,1.0);
	//double lmax = 158.0;
	//double ave = 0.0f;

	generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
	for (int i = 0; i < 10000; i++){
		double v = lognormal(generator) ;
		std::cout << i << " " << v << std::endl;
	}
	//ave /= 10000;
	//std::cerr << std::endl << ave << std::endl;

    return  EXIT_SUCCESS;
}
