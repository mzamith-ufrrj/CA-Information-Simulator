#include <Agent.hpp>
#include <cassert>
#include <iostream>
#include <CellularAutomataI.hpp> 
#include <cmath>
#include <cstring>
#include <boost/container/vector.hpp>
#include <boost/range/algorithm/find.hpp>

using namespace std;

Agent::Agent():mS0(0), mS1(0), mTime(0.0f), mID(0), mKDist(0.0f),
mDelta(0.0f), mAlpha(0.0f), mP_Epsilon(0.0f), mN_Epsilon(0.0f),
mNeighbors(NULL),mNNeightbors(0), mMemory(0.0f){
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
								double *mt,
								double *nt,
								boost::container::vector <unsigned int> *neighbors,
								stList *list){
	//boost::container::vector <unsigned int>::iterator it;
	double auxMt = *mt;
	double auxNt = *nt;
    if (k == 0) return;
    if (list == NULL) return;

    //cout << "\t\t1" << endl;
    stList *ptr = list;
    while (ptr != NULL){   // stoppping by immediately neighbor
    	Agent *agent = reinterpret_cast<Agent*> (ptr->agent);
    	unsigned int id = agent->getID();

    	auto ret = boost::find(*neighbors, id);
    	if (ret == neighbors->end()){
			if (agent->getState() == CellularAutomataI::ABANDONED){
				auxNt++;
			}else if (agent->getState() == CellularAutomataI::INTERESTED){
				auxMt++;
			}
			neighbors->push_back(id);
		}//if (ret == neighbors->end()){
    	ptr = ptr->next;
    }//while (ptr != NULL){

   // cout << "\t\t2" << endl;
    ptr = list;
    while (ptr != NULL){
    	Agent *agent = reinterpret_cast<Agent*> (ptr->agent);
    	stList *list  = agent->getNeighborsList();
    	getInterestedAndAbandoned(k-1, &auxMt, &auxNt, neighbors, list);
    	ptr = ptr->next;
    }

    *mt = auxMt;
    *nt = auxNt;
}// getInterestedAndAbandoned(size_t k, float *mt, float *nt,

void Agent::unaware2interested_concerned(void){
    double mt = 0.0f, nt = 0.0f, pt = 0.0f, partA, partB;
    size_t k = static_cast<size_t>(mKDist);
    boost::container::vector <unsigned int> neighbors;
    neighbors.clear();
    neighbors.push_back(this->getID());
    getInterestedAndAbandoned(k, &mt, &nt, &neighbors, mNeighbors);

    //part one of equation
    {
    	partA = 1.0 - mAlpha;
    	partA = (1.0 - pow(partA, mt)) * 0.5;
    }
    //part two of equation
    {
    	double a = 1.0 - exp(-mt/mKDist);
    	double b = exp(-nt/mKDist) - 1.0;
    	partB = ((a + b) / (1.0 - 1.0/exp(1.0))) * 0.5;

    }

    //pt is propagation factor
    pt = partA + partB;
    assert((pt >= 0.0f) && (pt <= 1.0f));

    double probP = CellularAutomataI::getRandomNormalized();

    if (probP < pt)
    	mS1 = CellularAutomataI::INTERESTED;
    else{
    	mS1 = CellularAutomataI::CONCERNED;
    	mMemory = 0.0f;
    }


}//Agent::unaware2interested(void){

void Agent::concerned2interested_abandoned(void){
	//stopping by immediately neighbors to that I can check they are
    stList *ptr = mNeighbors;
    bool has_interesed  = false;

    while (ptr != NULL){   // stoppping by immediately neighbor
    	Agent *agent = reinterpret_cast<Agent*> (ptr->agent);
    	//unsigned int state = agent->getState();
    	int iTime = static_cast<int>(agent->getElapsedTime());
/*
    	if ((state == CellularAutomataI::INTERESTED) && (iTime == 1))
    		mMemory++;
    	else if (state != CellularAutomataI::INTERESTED)
    		mMemory--;
*/
    	if ((iTime == 1)){
    		has_interesed = true;
    		mMemory++;
    	}
    	ptr = ptr->next;
    }//while (ptr != NULL){
    if (!has_interesed)
    	mMemory--;

    if (mMemory >= mP_Epsilon){
    	mS1 = CellularAutomataI::STATES::INTERESTED;
    	//cerr << "CellularAutomataI::STATES::INTERESTED" << endl;
    }
    else if (mMemory < mN_Epsilon){
    	mS1 = CellularAutomataI::STATES::ABANDONED;
    	//cerr << "CellularAutomataI::STATES::ABANDONED" << endl;
    }else
    	mS1 = mS0;
}//void Agent::concernedState(void){

void Agent::interested2abandoned(void){
	double mt = 0.0f, nt = 0.0f;
	size_t k = static_cast<size_t>(mKDist);
	boost::container::vector <unsigned int> neighbors;
	neighbors.clear();
	neighbors.push_back(this->getID());
	getInterestedAndAbandoned(k, &mt, &nt, &neighbors, mNeighbors);

	double a = exp(-mTime) * (mt / mKDist);
    if (a < mDelta){
        mS1 = CellularAutomataI::STATES::ABANDONED;
        mTime = 0.0f;
    }else{
    	mTime++;
    }
}//void Agent::interested2abandoned(void){
void Agent::rule(void){

    assert(mS0 != CellularAutomataI::STATES::ZEROSTATE);

    if (mS0 == CellularAutomataI::STATES::INTERESTED){
    	interested2abandoned();
        return;
    }//if (mState == CellularAutomataI::STATES::INTERESTED){

    if (mS0 == CellularAutomataI::STATES::UNAWARE){
    	unaware2interested_concerned(); //ok
        return;
    }//if (mState == CellularAutomataI::STATES::UNAWARE){

    if (mS0== CellularAutomataI::STATES::CONCERNED){
    	concerned2interested_abandoned();
        return;
    }//(mState == CellularAutomataI::STATES::CONCERNED){


}

void Agent::update(void){ mS0 = mS1; }

void Agent::setState(unsigned int s){
	if ((CellularAutomataI::INTERESTED == s) && (CellularAutomataI::INTERESTED != mS0)){
		mTime = 1;
	}
	mS0  = s;
};

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
