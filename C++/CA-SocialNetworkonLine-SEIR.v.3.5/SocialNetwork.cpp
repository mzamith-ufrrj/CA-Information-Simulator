#include <SocialNetwork.hpp>
#include <cassert>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <iomanip>
#include <algorithm>
#include <jsoncpp/json/json.h>

using namespace Json;
using namespace std;
SocialNetwork::SocialNetwork(){}

SocialNetwork::~SocialNetwork(){}
void SocialNetwork::initMem(void){

	mCellX = mCellY = mN_Nodes =  mTimeStep = 0;
	mVetProb = NULL ; mAgents = NULL; 	mVerbose = true;
	mAveAlpha = mN_Alpha = mAveGamma = mN_Gamma = 0.0;
	mAlpha_Rumor_MI =  mAlpha_Rumor_SIG = mGamma_Rumor_MI = 0.0;
	mGamma_Rumor_SIG =  mAveEpsilon =  mN_Epsilon = 0.0;
    mAveOutDegree =  mAveInDegree = mMaxOutDegree = mMaxInDegree = 0.0;
	mP_Infected = 0.0;mN_Infected = 0;mInitNetworkOpt = "";
	mLambda = 0.0;

	mGen.seed(std::chrono::system_clock::now().time_since_epoch().count());
}
void SocialNetwork::clearMem(void){
    if (mVetProb != NULL) free(mVetProb);
    mVetProb = NULL;
    for (uint64_t i = 0; i < mN_Nodes; i++){
    	mAgents[i].clearMem();
    }

    free(mAgents);
}

/*! \brief Load file Social Network configuration - JSON format
 *  @param filename JSON file name.
 */
void SocialNetwork::loadConfig(const std::string& filename){
    Value root;
    fstream input;

    if (mVerbose) cout << "Load config from file: [" << filename << "]" << endl;

    input.open(filename);
    assert(input.is_open());
    input >> root;
    input.close();

    std::string name =  root["global-params"].get("simulation-name", "UTF-8" ).asString();
    mVerbose = root["global-params"].get("verbose", "UTF-8" ).asBool();
    mLogFileStatistic = std::string("statistic." + name + ".csv");
    mFileStateAgent   = std::string("agent." + name + ".ppm");
    mFileGraph        =  root["global-params"].get("instance", "UTF-8" ).asString();

    mP_Infected = root["global-params"].get("infected", "UTF-8" ).asDouble();

	mTimeStep    = root["global-params"].get("timesteps", "UTF-8" ).asUInt();

    //mEpsilon       = root["global-params"].get("epsilon", "UTF-8" ).asDouble();

    mAlpha_Rumor_MI = root["global-params"].get("alpha-rumor-mi", "UTF-8" ).asDouble();
    mAlpha_Rumor_SIG  = root["global-params"].get("alpha-rumor-sig", "UTF-8" ).asDouble();
    mGamma_Rumor_MI = root["global-params"].get("gamma-rumor-mi", "UTF-8" ).asDouble();
    mGamma_Rumor_SIG  = root["global-params"].get("gamma-rumor-sig", "UTF-8" ).asDouble();

    mCellX = root["global-params"].get("cell-x", "UTF-8" ).asUInt();
    mCellY = root["global-params"].get("cell-y", "UTF-8" ).asUInt();
    mLambda  = root["global-params"].get("lambda", "UTF-8" ).asDouble();
    mInitNetworkOpt   =  root["global-params"].get("infected_init_order", "UTF-8" ).asString();

    if (mVerbose) print();
}


/*! \brief Simulation initial condition
 *
 */
void SocialNetwork::initialCondition(void){

    double K_total = 0.0f, K = 0.0f, add = 0.0f;
    uint64_t source = 0, target = 0;

    mUniform = std::uniform_real_distribution<double>(0.0, 1.0);
    mN_Alpha_Rumor  = std::lognormal_distribution<double>(mAlpha_Rumor_MI, mAlpha_Rumor_SIG);
    mN_Gamma_Rumor  = std::lognormal_distribution<double>(mGamma_Rumor_MI, mGamma_Rumor_SIG);


    std::string line;
    std::stringstream buffer;
    //Creating graph from file
    std::cout << "1 - Loading graph - filename:" << mFileGraph;
    std::ifstream graphFile;
    auto start_time = std::chrono::high_resolution_clock::now();
    	graphFile.open(mFileGraph, std::fstream::in);
    	assert(graphFile.is_open());
    	buffer << graphFile.rdbuf();
    	graphFile.close();
	auto stop_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> total_time = stop_time - start_time;

    std::cout << " [OK] " << std::endl << "\t - Load elapsed time:                    " << setw(8) << fixed << setprecision(4) <<  total_time.count() << std::endl;
    std::cout.flush();

    std::cout << "2 - initialCondition" << std::endl;
	std::cout.flush();
	start_time = std::chrono::high_resolution_clock::now();
    buffer >> line;
    if (!buffer.eof()){
    	if (line.compare("*Vertices") == 0){
    		buffer >> mN_Nodes;
    			assert(posix_memalign(reinterpret_cast<void**>(&mAgents), ALIGN, mN_Nodes *  sizeof(Agent)) == 0);
    		    int64_t id = 0;
    			for (uint64_t i =  0; i < mN_Nodes; i++){
    				mAgents[i].initMem();
    				mAgents[i].setID(id++);
    				mAgents[i].setStates(STATES::SUSCEPTIBLE);
    			}//for (unsigned int i = 0; i < mN_Nodes; i++){
				//bzero(mAgents, mN_Nodes *  sizeof(Agent));
    			buffer >> line;
    		}//if (line.compare("*Vertices") == 0){


    	    if (line.compare("*Arcs") == 0){
    	    	bool reading = true;
    			do{
    				buffer >> source;
    				buffer >> target;

    				if (!buffer.eof()){
						assert((source > 0) && (target > 0));
						add_relation(source-1, target-1, mAgents, mN_Nodes);

    				}else
    					reading = false;

    				//std::cout << source << "->" << target << std::endl;
    				//add_edge(source, target, *pGraph);
    			}while(reading);
    	    }//if (line.compare("*Arcs") == 0){


    }//if (!buffer.eof()){
    stop_time = std::chrono::high_resolution_clock::now();
    total_time = stop_time - start_time;
    std::cout << "\t - Parse and list creation elapsed time: " << setw(8) << fixed << setprecision(4) << total_time.count() << std::endl;
    std::cout.flush();

    start_time = std::chrono::high_resolution_clock::now();
    mAveAlpha = mN_Alpha = mAveGamma = mN_Gamma = 0.0;
    //Selecting spreaders in according the most connected nodes in in_degree
    //-------------------------------------------------------------------------
    assert(posix_memalign(reinterpret_cast<void**>(&mVetProb), ALIGN, mN_Nodes *  sizeof(double)) == 0);
    bzero(mVetProb, mN_Nodes *  sizeof(double));



    K_total = 0.0f, K = 0.0f, add = 0.0f;
    for (uint64_t i = 0; i < mN_Nodes; i++)
    	K_total += mAgents[i].getInDegree(); //K_total += static_cast<double>(boost::in_degree(i, *pGraph));

    K = mAgents[0].getInDegree();
    add = K / K_total;
    mVetProb[0] = add;
    for (uint64_t i = 1; i < mN_Nodes; i++){
    	K = mAgents[i].getInDegree(); //K = static_cast<double>(boost::in_degree(i, *pGraph));
        add += K / K_total;
        mVetProb[i] = add;
    }//
    stop_time = std::chrono::high_resolution_clock::now();
    total_time = stop_time - start_time;
    std::cout << "\t - Vector of probabilities elapsed time: " << setw(8) << fixed << setprecision(4) <<  total_time.count() << std::endl;
    std::cout.flush();
    //-------------------------------------------------------------------------
    //Obtaining the in-degree and out-degree average.
    //-----------------------------------------------------------------------------------------------------------------------------------------------------
    mMaxOutDegree = mMaxInDegree = 0.0f;
    mAveOutDegree =  mAveInDegree = 0.0f;
    for  (uint64_t node = 0; node < mN_Nodes; node++){

        double in_degree = mAgents[node].getInDegree();
        double out_degree = mAgents[node].getOutDegree();
        if (in_degree > mMaxInDegree) mMaxInDegree = in_degree;
        if (out_degree > mMaxOutDegree) mMaxOutDegree = out_degree;
        mAveInDegree += in_degree;
        mAveOutDegree += out_degree;


    }//for  (uint64_t node = 0; node < mN_Nodes; node++){
    double N = static_cast<double>(mN_Nodes);
    mAveInDegree /= N;
    mAveOutDegree /= N;

    std::cout << "\t - Maximum ( In:" << setprecision(1) << mMaxInDegree << ", Out:" << mMaxOutDegree <<  ")" <<  std::endl;

    std::cout << "\t - Average ( In:" << setprecision(1) << mAveOutDegree << ", Out:" << mAveInDegree <<  ")" <<  std::endl;

    //-----------------------------------------------------------------------------------------------------------------------------------------------------
    start_time = std::chrono::high_resolution_clock::now();
    double value = 0.0;
    if (mP_Infected < 1.0){
        value = floor(mP_Infected * static_cast<double> (mN_Nodes));
    }else{
        value = floor(mP_Infected);
    }



    mN_Infected = static_cast<uint64_t> (value);
    if (mN_Infected == 0) mN_Infected = 1;
    std::cout << "\t - Infected: [" <<  mN_Infected <<  "]" << std::endl;
    if (mInitNetworkOpt.compare("rand") == 0){
        for  (uint64_t node = 0; node < mN_Infected; node++){

            bool selected = false;
            do{
                double p =  mUniform(mGen);
                add = mVetProb[0];
                uint64_t j = 1;
                while (p > add)
                    add = mVetProb[j++];
                j--;
                if (mAgents[j].getState() == STATES::SUSCEPTIBLE){
                    mAgents[j].setStates(STATES::INFECTED);
                    selected = true;
                }
            }while (!selected);
        }//for  (uint64_t node = 0; node < mSpreaders; node++){
    }else{ //if (mInitNetworkOpt.compare("rand") == 0){
        //Creating index to order the Agent vector
        st_index *idx = NULL;
        assert(posix_memalign(reinterpret_cast<void**>(&idx), ALIGN, mN_Nodes *  sizeof(st_index)) == 0);
        bzero(idx, mN_Nodes *  sizeof(st_index));
        for (uint64_t i = 0; i < mN_Nodes; i++){
            idx[i].index = i;
            idx[i].key   = mAgents[i].getInDegree();
        }

        if (mInitNetworkOpt.compare("asc") == 0){
            std::sort(&(idx[0]), &(idx[mN_Nodes-1]), [](st_index &a, st_index &b) {
                return b.key < a.key;
            });
        }else if (mInitNetworkOpt.compare("desc") == 0){
            std::sort(&(idx[0]), &(idx[mN_Nodes-1]), [](st_index &a, st_index &b) {
                return b.key > a.key;
            });
        }

        for  (uint64_t node = 0; node < mN_Infected; node++){

            mAgents[idx[node].index].setStates(STATES::INFECTED);
        }
        free(idx);
    }


    stop_time = std::chrono::high_resolution_clock::now();

	total_time = stop_time - start_time;
    std::cout << "\t - First agents infected elapsed time:   " << setw(8) << fixed << setprecision(4) << total_time.count() << std::endl;
	std::cout.flush();


}//void SocialNetwork::initialCondition(void){

void SocialNetwork::exec(void){
	std::cout << "3 - update -" ;
	std::cout.flush();
    statistic(0);

	std::cout <<" U[";
	std::cout.flush();
	uint64_t mark = mTimeStep / 10;
	for (uint64_t t = 1; t <= mTimeStep; t++){
		update(t);
		statistic(t);
		if (( t % mark ) == 0){
			std::cout << ".";
			std::cout.flush();
		}
	}//for (uint64_t t = 1; t < mTimeStep; t++){
	std::cout << "]" << std::endl;
}//SocialNetwork::exec(void){


void SocialNetwork::statistic(uint64_t timestep){
	fstream logout;
	struct stat buffer;
	size_t state_statistic[4];
	state_statistic[0] = state_statistic[1] = state_statistic[2] = state_statistic[3] = 0;

	for (uint64_t i = 0; i < mN_Nodes; i++){
		uint64_t opt = mAgents[i].getState();
		assert(opt != ZEROSTATE);
		state_statistic[opt-1]++;
	}


	if (stat(mLogFileStatistic.c_str(), &buffer) == -1){
		logout.open(mLogFileStatistic, fstream::trunc|fstream::out);
		assert(logout.is_open());
		logout << "time;susceptible;exposed;infected;recovered;ave_alpha;num_alpha;ave_gamma;num_gamma;ave_epsilon;num_epsilon" << endl;
	}else{
		logout.open(mLogFileStatistic, fstream::app|fstream::out);
		assert(logout.is_open());
	}

    mAveAlpha /= mN_Alpha;
    mAveGamma /= mN_Gamma;
    mAveEpsilon /= mN_Epsilon;
    if (isnan(mAveAlpha)) mAveAlpha = 0.0;
    if (isnan(mAveGamma)) mAveGamma = 0.0;
    if (isnan(mAveEpsilon)) mAveEpsilon = 0.0;

	logout << timestep << ";" << state_statistic[0] << ";";
	logout << state_statistic[1] << ";" << state_statistic[2] << ";" << state_statistic[3] << ";" << \
	        mAveAlpha << ";" << mN_Alpha << ";" << mAveGamma << ";" << mN_Gamma  <<  ";" << mAveEpsilon << ";" << mN_Epsilon << endl;

	logout.close();
    mAveAlpha = mN_Alpha = mAveGamma = mN_Gamma = mN_Epsilon = mAveEpsilon = 0.0;

}//void SocialNetwork::statistic(void){


void SocialNetwork::update(uint64_t t){
		//std::cout << "update 1" << std::endl; std::cout.flush();
		for (uint64_t i = 0; i < mN_Nodes; i++){
			switch (mAgents[i].getState()){
				case SUSCEPTIBLE:susceptible2NS(i, t);  break;//std::cout << "susceptible2NS 1" << std::endl; std::cout.flush(); break;
				case EXPOSED: exposed2NS(i, t); break;//std::cout << "exposed2NS 1" << std::endl; std::cout.flush(); break;
				case INFECTED:infected2NS(i, t);  break;//std::cout << "infected2NS 1" << std::endl; std::cout.flush(); break;
				//case RECOVERED:r = 0xFF; g = 0xFF; b = 0x00;	break;
			}//switch (opt){
		}


		for (uint64_t i = 0; i < mN_Nodes; i++){
			mAgents[i].update();
		}



}//void SocialNetwork::update(void){

double SocialNetwork::buildEpsilon(uint64_t node, uint64_t iTime){
    double dTime  = static_cast<double>(iTime);
    double epsilon = 1.0 - exp(-mLambda * dTime);


    assert(!isnan(epsilon));
    assert(!isinf(epsilon));

	return epsilon;
}

double SocialNetwork::buildAlpha(uint64_t node, uint64_t iTime){

		double inf_k = 0.0, rec_k = 0.0, alpha = 0.0;
		st_NeighborNode *LNodes = mAgents[node].getOutList(); //List of I'm connected

		while (LNodes != NULL){
			//k++;
			uint64_t source = LNodes->index;
			if (mAgents[source].getState() == INFECTED){
                double k = mAgents[source].getInDegree();
			          inf_k += k;
            }else if (mAgents[source].getState() ==  RECOVERED){
                double k = mAgents[source].getInDegree(); //How many connections are in my neighbor
                rec_k += k;
            }
            //match.push_back(source);
            LNodes = LNodes->next;
        }
    
    double p1 = mN_Alpha_Rumor(mGen); //mW_Alpha;


    double t1 = static_cast<double>(iTime);

    if (iTime > 0){
        t1 /= p1;

    }else{
        t1 = ERROR  / p1;

    }

    alpha = (1.0 - exp(-((inf_k/mMaxInDegree) * t1))); //alpha = (s/k) + (1.0 - exp(-c/k));
    //alpha -= (1.0 - exp(-((rec_k/mMaxInDegree) * t1)));
    
    assert(!isnan(alpha) && !isinf(alpha));
    if (alpha < ERROR)  alpha =  0.0;
    return alpha;
}


double SocialNetwork::buildGamma(uint64_t node, uint64_t iTime){
    
    double inf_k = 0.0, rec_k = 0.0, gamma = 0.0;
    st_NeighborNode *LNodes = mAgents[node].getOutList();
    
    
    while (LNodes != NULL){
      uint64_t source = LNodes->index;
      //if (mAgents[source].getState() == INFECTED){
      //  double k = mAgents[source].getInDegree();
      //  inf_k += k;
      //}else
      if (mAgents[source].getState() ==  RECOVERED){
        double k = mAgents[source].getInDegree();
        rec_k += k;
      }

      //match.push_back(source);
      
      LNodes = LNodes->next;
    }
    
    double p1 = mN_Gamma_Rumor(mGen); //mW_Alpha;

    double t1 = static_cast<double>(iTime);

    if (iTime > 0){
            t1 /= p1;
    }
    else{
      t1 = ERROR  / p1;
    }
    
    //gamma = -(1.0 - exp(-((inf_k/mMaxInDegree) * t2))); //alpha = (s/k) + (1.0 - exp(-c/k));
    gamma = (1.0 - exp(-((rec_k/mMaxInDegree) * t1)));
    assert(!isnan(gamma) && !isinf(gamma));
    if (gamma < ERROR)
      gamma =  1.0 - exp(-static_cast<double>(iTime));
    assert((gamma >= 0) &&  (gamma <= 1));
    return gamma;
}

void SocialNetwork::susceptible2NS(uint64_t node, uint64_t itime){

    double epsilon = buildEpsilon(node, itime);
    double alpha = buildAlpha(node, itime);
    double prob = mUniform(mGen);

    mAveAlpha += alpha;
    mN_Alpha++;
    
    mAveEpsilon += epsilon;
    mN_Epsilon++;



    double a = alpha;
    double b = a + epsilon * (1.0 - alpha);
    if (prob < a)
        mAgents[node].setState(INFECTED);
    else if ((a <= prob) && (prob < b))
        mAgents[node].setState(EXPOSED);
    else
        mAgents[node].setState(mAgents[node].getState());

}//void SocialNetwork::susceptible2NS(uint64_t node){

void SocialNetwork::exposed2NS(uint64_t node, uint64_t itime){
	double gamma = buildGamma(node, itime);
	assert((gamma >= 0.0) && (gamma <= 1.0));
	double alpha =  buildAlpha(node, itime);
	if (alpha < 0.0){
		std::cerr << alpha << std::endl;
		exit(1);
	}
    mAveAlpha += alpha;
    mN_Alpha++;

    mAveGamma += gamma;
    mN_Gamma++;


	double prob = mUniform(mGen);
    double a = gamma;
    double b = gamma + (alpha * (1.0 - gamma));

    if (prob < a)
        mAgents[node].setState(RECOVERED);
    else if ((a <= prob) && (prob <= b))
        mAgents[node].setState(INFECTED);
    else
        mAgents[node].setState(mAgents[node].getState()); //Keep the state

}

void SocialNetwork::infected2NS(uint64_t node, uint64_t itime){

	//if (itime > 10)
	//	std::cerr << "nop" ;
	double gamma = buildGamma(node, itime);
	assert((gamma >= 0.0) && (gamma <= 1.0));


    mAveGamma += gamma;
    mN_Gamma++;
	double prob = mUniform(mGen);
	//std::cerr << "gamma: " << gamma << std::endl;
	if (prob < (gamma)) mAgents[node].setState(RECOVERED);
	else mAgents[node].setState(mAgents[node].getState()); //Keep the state
}



void SocialNetwork::save2ppm(void){
//char *buff = NULL;
/*

	assert(posix_memalign((void**) &buff, ALIGN, mN_Nodes * mN_Nodes *  sizeof(char) * 3) == 0);
	//const uint64_t offset = 3;
	char r, g, b;
	for (uint64_t j = 0; j < mN_Nodes; j++){
		for (uint64_t i = 0; i < mN_Nodes; i++){
			uint64_t p = j * mN_Nodes + i;

			if ((j != i) && (mAdjMatrix[p])){

				//unsigned int opt = mAgents[i].getState();
				switch (mAgents[i].getState()){
					case SUSCEPTIBLE:r = 0x00; g = 0xFF; b = 0x00;break;
					case EXPOSED:r = 0x00; g = 0x00; b = 0xFF;break;
					case INFECTED:r = 0xFF; g = 0x00; b = 0x00;break;
					case RECOVERED:r = 0xFF; g = 0xFF; b = 0x00;	break;
				}//switch (opt){
			}else if (i == j){
				 r = g = b = 0xFF;
			}else if (!mAdjMatrix[p]){
				 r = g = b = 0x00; // Black in image background
			}
			buff[3 * p] = r;
			buff[3 * p + 1] = g;
			buff[3 * p + 2] = b;
		}//for (uint64_t i = 0; i < mN_Nodes; i++){

	}//for (uint64_t j = 0; j < mN_Nodes; j++){
	*/
	/*
	for (uint64_t i = 0; i < mN_Nodes * mN_Nodes * 3; i+=3){
		char r = 0x00;
		char g = 0x00;
		char b = 0x00;

		buff[i] = r;
		buff[i + 1] = g;
		buff[i + 2] = b;

		//char c = (mAdjMatrix[i] == true) * 0xFF;
		//buff[i] = c;
	}

	fstream output;
	output.open(mLogFileStateAgent, fstream::binary|fstream::out);
	assert(output.is_open());
	output << "P6\n";
	output << mN_Nodes << " " << mN_Nodes << "\n";
	output << "255\n";
	const char *ptr = reinterpret_cast<const char *>(buff);
	output.write(ptr, mN_Nodes * mN_Nodes * sizeof(char) * 3);
	output.close();
	free(buff);
	cout << "File: [" << mLogFileStateAgent << "] saved [OK]" << endl;
	*/
}

void SocialNetwork::printNetworkStatistic(void){
	/*
    boost::container::vector <uint64_t> in_hist, out_hist;
    uint64_t iK_in_max = 0, iK_out_max = 0;

    for (uint64_t i = 0; i < mN_Nodes; i++){
        uint64_t iK = static_cast<uint64_t>(boost::in_degree(i, *pGraph));
        if (iK > iK_in_max) iK_in_max = iK;

        iK = static_cast<uint64_t>(boost::out_degree(i, *pGraph));
        if (iK > iK_out_max) iK_out_max = iK;
        //std::cout << "Node(" << i << ") -> K = " << iK << std::endl;
    }//
    std::cout << "Max  in K = " << iK_in_max << std::endl;
    std::cout << "Max out K = " << iK_out_max << std::endl;
    in_hist.resize(iK_in_max+1);
    out_hist.resize(iK_out_max+1);
    for (uint64_t i = 0; i < mN_Nodes; i++){
        uint64_t iK = static_cast<uint64_t>(boost::in_degree(i, *pGraph));
        in_hist[iK]++;
        iK = static_cast<uint64_t>(boost::out_degree(i, *pGraph));
        out_hist[iK]++;
    }


    std::string filename = mFileGraph + ".in_degree";
    std::fstream output;
    std::cout << "- File: " << filename;
    output.open(filename, std::fstream::out|std::fstream::trunc);
	assert(output.is_open());
	output << "degree;frequency" << std::endl;
	for (uint64_t n = 0; n < in_hist.size(); n++)
		output << n+1 << ";" << in_hist[n] << std::endl;
	output.close();
    std::cout << " [OK]" << std::endl;

    filename = mFileGraph + ".out_degree";
	output.open(filename, std::fstream::out|std::fstream::trunc);
	std::cout << "- File: " << filename;
	assert(output.is_open());
	output << "degree;frequency" << std::endl;
	for (uint64_t n = 0; n < out_hist.size(); n++)
		output << n+1 << ";" << out_hist[n] << std::endl;
	output.close();
    std::cout << " [OK]" << std::endl;

    filename = mFileGraph + ".topology";
	output.open(filename, std::fstream::out|std::fstream::trunc|std::fstream::binary);
	std::cout << "- File: " << filename;
	assert(output.is_open());
	output.write(reinterpret_cast<char *>(&mN_Nodes), sizeof(uint64_t));
    auto [edge_begin, edge_end] = boost::edges(*pGraph);
    for (auto it = edge_begin; it != edge_end; ++it) {
        uint64_t source = reinterpret_cast<uint64_t>(boost::source(*it, *pGraph)); // Vértice de origem
        uint64_t target = reinterpret_cast<uint64_t>(boost::target(*it, *pGraph)); // Vértice de destino
        output.write(reinterpret_cast<char *>(&source), sizeof(uint64_t));
        output.write(reinterpret_cast<char *>(&target), sizeof(uint64_t));
    }

	output.close();
    std::cout << " [OK]" << std::endl;

*/
}//void SocialNetwork::printNetworkStatistic(void){


void SocialNetwork::print(void){
	//bool flag = mN_Nodes == (mCellX * mCellY);
    cout << endl << "Simulation of online information" << endl;
	cout << " - Config:" << endl;
	//cout << "\tNodes(" << mN_Nodes << ") = " << mCellX << "x" <<  mCellY << " = ["  << std::boolalpha << flag << "] | Timesteps(" << mTimeStep << ")" << endl;
	//cout << "\tThe average degree: " << setw(8) << fixed << setprecision(4) << mAve_degree  << endl;
	//cout << "\tThe maximum degree: " << setw(8) << fixed << setprecision(4) << mMax_degree  << endl;
	//cout << "\tEpsion rate       : (" << setw(8) << fixed << setprecision(4) << mEpsilon  << ")" << endl;
	cout << "\tAlpha Log-Normal  : (" << setw(8) << fixed << setprecision(4) << mAlpha_Rumor_MI  << "," << mAlpha_Rumor_SIG << ")" << endl;
	cout << "\tGamma Log-Normal  : (" << setw(8) << fixed << setprecision(4) << mGamma_Rumor_MI  << "," << mGamma_Rumor_SIG << ")" << endl;
	//cout << "\t Infected : (" << setw(8) << fixed << mP_Infected << " %)" << endl;
	cout << "\t Infected order : [" << mInitNetworkOpt << "]" << endl;
	cout << "\tLog files: " << endl;
	cout << "\t - Log file statistic:      [" << mLogFileStatistic << "]" << endl;
	cout << "\t - State file state agents: [" << mFileStateAgent << "]" <<endl;
	//cout << "\t - State file graph:        [" << mStateFileGraph << "]" << endl;
	cout << "\t - File graph info:         [" << mFileGraph << "]" << endl << endl << endl;

	/*for (uint64_t i = 0; i < mN_Nodes; i++){
		mAgents[i].printNeighbors();
	}//for (uint64_t i = 0; i < mN_Nodes; i++){
	*/
}
