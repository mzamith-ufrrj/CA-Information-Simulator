#include <Agent.hpp>
#include <iostream>
#include <cassert>
#include <cmath>
#include <cstring>

Agent::Agent():mS0(0),mS1(0),mID(0), mSpreader(false){
#ifdef _DEBUG
    std::cout << "Agent()" << std::endl;
#endif
}
Agent::~Agent(){}
void Agent::update(void){ mS0 = mS1;}
void Agent::setStates(uint64_t s){	mS0 = mS1 = s;}
