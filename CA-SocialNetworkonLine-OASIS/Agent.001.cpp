#include <Agent.hpp>
#include <cassert>
#include <iostream>
#include <CellularAutomataI.hpp> 
#include <cmath>
#include <cstring>
#include <boost/container/vector.hpp>
#include <boost/range/algorithm/find.hpp>
using namespace boost::container;
using namespace std;

Agent::Agent():mState(0), mTime(0.0f), mID(0), mKDist(0.0f),
mDelta(0.0f), mAlpha(0.0f), mEpsilon(0.0f), mNeighbors(NULL),
mNNeightbors(0){
//#ifdef _DEBUG
//    cout << "Agent()" << endl;
//#endif

}

Agent::~Agent(){
    stList *ptr = mNeighbors;
    stList *ptr1 = ptr;
    while (ptr!=NULL){
        ptr = ptr->next;
        free(ptr1);
        ptr1 = ptr;
    }
}

void Agent::getInterestedAndAbandoned(	size_t k,
								float *mt,
								float *nt,
								boost::container::vector <unsigned int> *neighbors,
								stList *list){
	//boost::container::vector <unsigned int>::iterator it;
    if (k == 0) return;
    if (list == NULL) return;

    //We need another stop critery

    Agent *agent = reinterpret_cast<Agent*> (list->agent);
    //Verified if this agent was considered in 
    unsigned int id = agent->getID();
    auto ret = boost::find(*neighbors, id);
    if (ret == neighbors->end()){
        if (agent->getState() == CellularAutomataI::ABANDONED){
            *nt++;
        }else if (agent->getState() == CellularAutomataI::INTERESTED){
            *mt++;
        }
        neighbors->push_back(id);
    }//if (ret == neighbors->end()){

    stList *ptr = agent->getNeighborsList();
	while (ptr != NULL){
		getInterestedAndAbandoned(k-1, mt, nt, neighbors, ptr);
		ptr = ptr->next;
	}
}// getInterestedAndAbandoned(size_t k, float *mt, float *nt,

void Agent::unaware2interested(void){
    float mt = 0.0f, nt = 0.0f;
    size_t k = mKDist;
    boost::container::vector <unsigned int> neighbors;
    neighbors.clear();
    neighbors.push_back(this->getID());

    stList *ptr = mNeighbors;
    while (ptr != NULL){
        getInterestedAndAbandoned(k, &mt, &nt, &neighbors, ptr);
        ptr = ptr->next;
    }

    cout << "root mt: " << mt << ", nt: " << nt << endl;
    cout << "\t Visited: " << neighbors.size() << " >:";
    for (auto i: neighbors){
    	cout << i << ",";
    }
    cout << endl;
}//Agent::unaware2interested(void){


void Agent::update(void){
    float mt = 3.0f;
    assert(mState != CellularAutomataI::STATES::ZEROSTATE);

    if (mState == CellularAutomataI::STATES::INTERESTED){
        float a = exp(-mTime) * (mt / mKDist);
        if (a < mDelta)
            mState = CellularAutomataI::STATES::ABANDONED;
        return;
    }//if (mState == CellularAutomataI::STATES::INTERESTED){

    if (mState == CellularAutomataI::STATES::UNAWARE){
        unaware2interested();
        return;
    }//if (mState == CellularAutomataI::STATES::UNAWARE){

    if (mState == CellularAutomataI::STATES::CONCERNED){

        return;
    }//(mState == CellularAutomataI::STATES::CONCERNED){

}



int Agent::setNeighborsAddr(Agent *a){

    
    stList *l = NULL, *ptr = NULL, *ptr1 = NULL;
    if (a == this) return -1;
    
    //It is the first
    if (mNeighbors == NULL){
        assert(posix_memalign((void**) &l, ALIGN, sizeof(stList)) == 0);
        l->agent = a;
        l->next = NULL;
        mNeighbors = l;
        mNNeightbors++;
        return  0;
    }

    ptr = mNeighbors;
    while (ptr != NULL){
        if (ptr->agent == a) 
            return -2;
        ptr1 = ptr;    
        ptr = ptr->next;
    }

    assert(posix_memalign((void**) &l, ALIGN, sizeof(stList)) == 0);
    l->agent = a;
    l->next = NULL;
    ptr1->next = l;
    mNNeightbors++;

    return 0;
}

void Agent::printNeighbors(void){
    cout << "I am: " << mID << " I'm relating to " << mNNeightbors << " neighbors" << endl;
    cout << "======================================" << endl;
    stList *ptr = mNeighbors;
    while (ptr != NULL){
        Agent *a = reinterpret_cast<Agent*> (ptr->agent);
        cout << " - " << a->getID() << endl;
        ptr = ptr->next;

    }
}
