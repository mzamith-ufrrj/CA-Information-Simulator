/*
 * File Main
 * Traffic Cellular Automata simulation  TCA-S
 */


#include <cstdlib>
#include <string>
#include <boost/container/vector.hpp>
//#include <boost/container/iterator.hpp>
#include <boost/range/algorithm/find.hpp>
#include <iostream>
using namespace boost::container;
using namespace std;
void add(boost::container::vector <unsigned int> *vet){
	vet->clear();
	vet->push_back(1);
	vet->push_back(3);
	vet->push_back(5);
}


int main(int ac, char **av){
	boost::container::vector <unsigned int> vet;
	boost::container::vector <unsigned int>::iterator it;
		add(&vet);
	vet.push_back(0);
	vet.push_back(42);
	//td::find(vector.begin,vector.end, element)

	for (auto var:vet){
		cout << var << endl;
	}


	it = boost::find(vet,53);
	cout << *it << endl;

	auto ret = boost::find(vet,0);
	if (ret == vet.end())
		cout << "Not found!" << endl;
	else
		cout << "Find value in the set!" << endl;

	//cout << ret << endl;
	//

   return EXIT_SUCCESS;
}

