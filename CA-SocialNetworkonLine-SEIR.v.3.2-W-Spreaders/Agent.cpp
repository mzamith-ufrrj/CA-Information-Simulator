#include <Agent.hpp>
#include <iostream>
#include <cassert>
#include <cmath>
#include <cstring>

Agent::Agent():mS0(0),mS1(0),mID(0), mSpreader(false),
mInDegree(0), mOutDegree(0), mInNode(NULL), mOutNode(NULL){
#ifdef _DEBUG
    std::cout << "Agent()" << std::endl;
#endif
}
Agent::~Agent(){}
void Agent::initMem(void){
	mS0 = mS1 = mID = 0;
	mSpreader = false;
	mInDegree = mOutDegree = 0;
	mInNode = mOutNode = NULL;
}
void Agent::clearMem(void){
	st_NeighborNode *ptr = mInNode,
			        *aux = NULL;
	while(ptr != NULL){
		aux = ptr;
		ptr = ptr->next;
		free(aux);
	}

	ptr = mOutNode,
	aux = NULL;
	while(ptr != NULL){
		aux = ptr;
		ptr = ptr->next;
		free(aux);
	}
}
void Agent::update(void){ mS0 = mS1;}
void Agent::setStates(uint64_t s){	mS0 = mS1 = s;}

 //This represents my followers, who connect me to know that I know
void Agent::addInNeighbor(uint64_t source){
	st_NeighborNode *ptr = NULL;
	assert(posix_memalign(reinterpret_cast<void**>(&ptr), ALIGN, sizeof(st_NeighborNode)) == 0);

	ptr->index = source;
	ptr->next = NULL;

	insert(&mInNode, ptr);
	mInDegree++;
};

//This represents person who I follow, who I connect to know that he/his know
void Agent::addOutNeighbor(uint64_t target){
	st_NeighborNode *ptr = NULL;
	assert(posix_memalign(reinterpret_cast<void**>(&ptr), ALIGN, sizeof(st_NeighborNode)) == 0);
	ptr->index = target;
	ptr->next = NULL;
	insert(&mOutNode, ptr);
	mOutDegree++;

};

void Agent::printNeighbors(void){
	std::cout << "printNeighbors - OUT: (" << mID << "," << mOutDegree << ") -> [";
	st_NeighborNode *ptr = mOutNode;
	while (ptr != NULL){
		std::cout << ptr->index;
		if (ptr->next != NULL) std::cout << ",";
		ptr = ptr->next;
	}//while (ptr != NULL){
	std::cout << "]" << std::endl;

	/*
	std::cout << "printNeighbors - IN: (" << mID << "," << mInDegree << ") -> [";
	ptr = mInNode;
	while (ptr != NULL){
		std::cout << ptr->index;
		if (ptr->next != NULL) std::cout << ",";
		ptr = ptr->next;
	}//while (ptr != NULL){
	std::cout << "]" << std::endl;
	*/

};
void Agent::insert(st_NeighborNode **top, st_NeighborNode*ptr){
	st_NeighborNode *listTop = *top;//, *aux = *top;
	ptr->next = listTop;
	*top = ptr;




	/*
	if (*top == NULL){
		*top = ptr;
		return;
	}//if (*top == NULL){

	if (ptr->index < listTop->index){
		ptr->next = listTop;
		*top = ptr;
		return;
	}//if (ptr->index < listTop){

	while ((listTop->index < ptr->index) && (listTop->next != NULL)){
		aux = listTop;
		listTop = listTop->next;
	}

	if (ptr->index <= listTop->index){
		assert(ptr->index < listTop->index);// Not allowed repeated relation
		aux->next = ptr;
		ptr->next = listTop;
		return;
	}

	if (listTop->next == NULL){
		listTop->next = ptr;
		return;
	}

	std::cerr << "[ERROR] undefined case file: " << __FILE__ << ":" << __LINE__ << std::endl;
	exit(-1);
	*/

};

//-----------------------------------------------------------------------------
void add_relation(uint64_t source, uint64_t target, Agent *agents, uint64_t nodes){
	assert((source < nodes) && (target < nodes));
	//std::cout << "\t" << source << " -> " << target << std::endl;
	//std::cout.flush();
	//Who I follow
	agents[source].addOutNeighbor(target);

	//Who is followed
	agents[target].addInNeighbor(source);
}
