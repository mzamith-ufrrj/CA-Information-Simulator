#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cassert>
int main (int ac, char **av){
	std::ifstream input;
	std::stringstream buffer;
	input.open("BA-Model-Nodes-1000000-power-1-connection-6.net", std::fstream::in);
	assert(input.is_open());
	buffer << input.rdbuf();
	input.close();
	std::string str = buffer.str();
	std::cout << str << std::endl;
	buffer >> str;
	std::cout << str;


	return EXIT_SUCCESS;
}
