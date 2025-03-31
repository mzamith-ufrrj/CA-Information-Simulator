#include <CellularAutomataI.hpp>
#include <cassert>
#include <fstream>
#include <cstddef>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <iomanip>
#include <jsoncpp/json/json.h>
using namespace Json;
using namespace std;
CellularAutomataI::CellularAutomataI():
mNodeX(0), mNodeY(0), mP_Epsilon(0.0f), mN_Epsilon(0.0f),
mAlpha(0.0f), mDelta(0.0f),
mNodes(0), mMaxNeighbors(0), mTimeStep(0), mVerbose(true){
    //srand ( time(NULL) );
    srand ( 42 );
}

CellularAutomataI::~CellularAutomataI(){
    if (mAgents != NULL){
        for (unsigned int i = 0; i < mNodes; i++){
            mAgents[i]->~Agent();
            free(mAgents[i]);
        }
            
        free(mAgents);

    }
   
}

void CellularAutomataI::exec(void){
	cout << "Start simulation: ";
	cout.flush();
	//cout << "initialCondition();";cout.flush();
	initialCondition();
	//cout << "[OK]" << endl; cout << "statistic();";	cout.flush();
	statistic();
	//cout << "[OK]" << endl;cout.flush();
	size_t mark = mTimeStep / 10;
	cout << "> ";
	for (size_t t = 1; t <= mTimeStep; t++){
		update();
		statistic(t);
		//cout << t << endl;
		//cout.flush();
		if (( t % mark ) == 0){
			std::cout << ".";
			std::cout.flush();
		}
	}//for (size_t t = 0; t < mTimeStep; t++){
	cout << endl;
}//void CellularAutomataI::

void CellularAutomataI::initialCondition(void){
    unsigned int i = mNodeX / 2;
    unsigned int j = mNodeY / 2;
    mAgents[j * mNodeX + i]->setState(STATES::INTERESTED);
    //cout << "> " << mAgents[j * mNodeX + i]->getID() << endl;
    for (unsigned int i = 0; i < mNodes; i++){
        size_t n = (getRandomInt() % mMaxNeighbors) + 1;
        size_t n1 = 0;
    
        bool inserted = false;
        while (!inserted){
    
            if (n1 < n){
                size_t pos = getRandomInt() % mNodes;
                if (pos != i){
                    Agent * addr = mAgents[pos];
                    int ret = mAgents[i]->setNeighborsAddr(addr);
                    addr->setNeighborsAddr(mAgents[i]);
                    if (ret == 0)
                        n1++;
                    
                }//if (pos != i){
            }//if (n1 < n){
            else
                inserted = true;
            
        }//if (n1 < n){
    }//for (unsigned int i = 0; i < mNodes; i++){  
}

void CellularAutomataI::statistic(size_t timestep){
	fstream logout;
	struct stat buffer;
	size_t state_statistic[4];
	state_statistic[0] = state_statistic[1] = state_statistic[2] = state_statistic[3] = 0;

	for (unsigned int i = 0; i < mNodes; i++){
		unsigned int opt = mAgents[i]->getState();
		assert(opt != CellularAutomataI::ZEROSTATE);
		state_statistic[opt-1]++;
	}


	if (stat(mLogFile.c_str(), &buffer) == -1){
		logout.open(mLogFile, fstream::trunc|fstream::out);
		assert(logout.is_open());
		logout << "time;width;height;unaware;concerned;interested;abandoned" << endl;
	}else{
		logout.open(mLogFile, fstream::app|fstream::out);
		assert(logout.is_open());
	}

	logout << timestep << ";" << mNodeX << ";" << mNodeY << ";" << state_statistic[0] << ";";
	logout << state_statistic[1] << ";" << state_statistic[2] << ";" << state_statistic[3] << endl;

	logout.close();
}//void CellularAutomataI::statistic(void){


void CellularAutomataI::update(void){

	for (unsigned int i = 0; i < mNodes; i++){
		mAgents[i]->rule();
	}//for (unsigned int i = 0; i < mNodes; i++){

	for (unsigned int i = 0; i < mNodes; i++){
		mAgents[i]->update();
	}//for (unsigned int i = 0; i < mNodes; i++){

}//void CellularAutomataI::update(void){

void CellularAutomataI::loadConfig(const string& filename){
    Value root; 
    fstream input;

    if (mVerbose) cout << "Load config from file: [" << filename << "]" << endl;
    input.open(filename);
    assert(input.is_open());
    input >> root;
    input.close();
    mLogFile     = root["global-params"].get("log-file", "UTF-8" ).asString();
    mP_Epsilon   = root["global-params"].get("p-epsilon", "UTF-8" ).asDouble();
    mN_Epsilon   = root["global-params"].get("n-epsilon", "UTF-8" ).asDouble();
	mAlpha       = root["global-params"].get("alpha", "UTF-8" ).asDouble();
    mDelta       = root["global-params"].get("delta", "UTF-8" ).asDouble();
    mNodeX       = root["global-params"].get("nodeX", "UTF-8" ).asUInt();
    mNodeY       = root["global-params"].get("nodeY", "UTF-8" ).asUInt();
    mTimeStep    = root["global-params"].get("timesteps", "UTF-8" ).asUInt();
	mKDist       = root["global-params"].get("k-distance", "UTF-8" ).asUInt();
    mMaxNeighbors = root["global-params"].get("n-neighbors", "UTF-8" ).asUInt();
    mNodes = mNodeX * mNodeY;
    
    assert(posix_memalign((void**) &mAgents, ALIGN, mNodes *  sizeof(Agent*)) == 0);
    
    //mAgents.clear();
    unsigned int id = 1;
    for (unsigned int i = 0; i < mNodes; i++){
        Agent *a = NULL;
        assert(posix_memalign((void**) &a, ALIGN, sizeof(Agent)) == 0);
        *a = Agent();
        mAgents[i] = a;
        mAgents[i]->setID(id++);
        mAgents[i]->setState(STATES::UNAWARE);
        mAgents[i]->setDelta(mDelta);
        mAgents[i]->setAlpha(mAlpha);
        mAgents[i]->setEpsilons(mP_Epsilon, mN_Epsilon);
        mAgents[i]->set_K_distance(mKDist);
        
        
    }//for (unsigned int i = 0; i < mNodes; i++){
    
}

void CellularAutomataI::print(void){
    cout << endl << "Simulation of online information" << endl;
	cout << "Config:" << endl;
	cout << "\tDomain( w:" << mNodeX << ", h:" << mNodeY << ", t:" << mTimeStep << " )" << endl;
	cout << "\tK-neighbors level: " << mKDist << " / Maximum neighbors: " << mMaxNeighbors << endl;
	cout << "\t           Alpha : " << fixed << setprecision(4) << mAlpha  << endl;
	cout << "\t        P-Epsilon: " << fixed << setprecision(4) << mP_Epsilon << endl;
	cout << "\t        N-Epsilon: " << fixed << setprecision(4) << mN_Epsilon << endl;
	cout << "\t           Delta : " << fixed << setprecision(4) << mDelta  << endl;


}


void CellularAutomataI::debug(void){
    //cout << "Oi" << endl;
    
    for (size_t j = 0; j < mNodeY; j++){
        for (size_t i = 0; i < mNodeX; i++){
            cout << mAgents[j * mNodeX + i]->getState();
        }
        cout << endl;
    }
    cout << "=============================================================" << endl;
    /*
    cout << "=============================================================" << endl;
    for (size_t i = 0; i < mNodes; i++){
        Agent *a = mAgents[i];
        if (a != NULL)
            a->printNeighbors();
    }
    */

}

void CellularAutomataI::debug1(void){
    //cout << "Oi" << endl;
/*
    for (size_t j = 0; j < mNodeY; j++){
        for (size_t i = 0; i < mNodeX; i++){
            cout << scientific << setw(15) << mAgents[j * mNodeX + i]->getAux() << " ";
        }
        cout << endl;
    }
    cout << "=============================================================" << endl;
    */
    /*
    cout << "=============================================================" << endl;
    for (size_t i = 0; i < mNodes; i++){
        Agent *a = mAgents[i];
        if (a != NULL)
            a->printNeighbors();
    }
    */

}
double CellularAutomataI::getRandomNormalized(void)         { return static_cast <double> (rand() % 10000 + 1) / 10000.0f;}
size_t CellularAutomataI::getRandomInt(void)                { return rand(); }
