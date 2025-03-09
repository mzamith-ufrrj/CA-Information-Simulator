#include <Agent.hpp>
#include <Tools.hpp>
#include <cassert>
#include <iostream>
#include <cmath>
#include <cstring>
//#include <boost/container/vector.hpp>
//#include <boost/range/algorithm/find.hpp>
//using namespace boost::container;
using namespace std;

Agent::Agent():mS0(0),mS1(0),mID(-1){
#ifdef _DEBUG
    std::cout << "Agent()" << std::endl;
#endif
}
Agent::~Agent(){}
void Agent::update(void){ mS0 = mS1;}
void Agent::setStates(unsigned int s){	mS0 = mS1 = s;}
