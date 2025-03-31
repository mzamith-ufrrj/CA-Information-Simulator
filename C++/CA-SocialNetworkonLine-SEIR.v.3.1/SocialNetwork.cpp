#include <SocialNetwork.hpp>
#include <cassert>
#include <fstream>
#include <cstddef>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <iomanip>
#include <jsoncpp/json/json.h>
#include <chrono>
#include <boost/range/algorithm/find.hpp>
#include <boost/container/vector.hpp>
#include <boost/range/algorithm/find.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/adjacency_matrix.hpp>
using namespace boost::container;
using namespace Json;
using namespace std;
SocialNetwork::SocialNetwork(): mN_Nodes(0), mN_Spreaders(0),
		mTimeStep(0), mVerbose(true), mVetProb(NULL),
		mEpsilon(0.0), mAveAlpha(0.0), mN_Alpha(0.0), mAveGamma(0.0), mN_Gamma(0.0),
		mAlpha_Rumor_Ave(0.0), mAlpha_Rumor_DP(0.0), mGamma_Rumor_Ave(0.0),
		mGamma_Rumor_DP(0.0), mCellX(0), mCellY(0){
    mGen.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

SocialNetwork::~SocialNetwork(){
    if (mVetProb != NULL) free(mVetProb);
    mVetProb = NULL;
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

    mLogFileStatistic = std::string("statistic." + name + ".csv");
    mFileStateAgent   = std::string("agent." + name + ".ppm");
    mFileGraph        =  root["global-params"].get("instancia", "UTF-8" ).asString();

    mN_Spreaders = root["global-params"].get("spreaders", "UTF-8" ).asUInt();
    mTimeStep    = root["global-params"].get("timesteps", "UTF-8" ).asUInt();

    mEpsilon       = root["global-params"].get("epsilon", "UTF-8" ).asDouble();

    mAlpha_Rumor_Ave = root["global-params"].get("alpha-rumor-ave", "UTF-8" ).asDouble();
    mAlpha_Rumor_DP  = root["global-params"].get("alpha-rumor-sd", "UTF-8" ).asDouble();
    mGamma_Rumor_Ave = root["global-params"].get("gamma-rumor-ave", "UTF-8" ).asDouble();
    mGamma_Rumor_DP  = root["global-params"].get("gamma-rumor-sd", "UTF-8" ).asDouble();

    mCellX = root["global-params"].get("cell-x", "UTF-8" ).asDouble();
    mCellY = root["global-params"].get("cell-y", "UTF-8" ).asDouble();


}

void SocialNetwork::createSpreader(uint64_t t){
    //uint64_t current = mSpreaders;
    boost::adjacency_matrix<boost::directedS> *pGraph = mGraph.get();
    double k_in = 0.0, k_out = 0.0, k_samples  = 0.0, H = 0.0;
	for (uint64_t i = 0; i < mN_Nodes; i++){
	    if (mAgents[i].getSpreader()){
	        k_in += static_cast<double>(boost::in_degree(i, *pGraph));
	        k_out += static_cast<double>(boost::out_degree(i, *pGraph));
	        k_samples++;
	    }//if (mAgents[i].getSpreader()){
    }//for (uint64_t i = 0; i < mN_Nodes; i++){
    k_in /= k_samples;
    k_out /= k_samples;
    H = k_in / k_out;
    if (isnan(H)) H = 0.0f;
    double inte = 0.9;
    double d_new_spreaders = static_cast<double>(mN_Spreaders) + H * exp(-static_cast<double>(t) * (1.0 - inte));
    uint64_t i_new_spreaders = static_cast<double>(floor(d_new_spreaders + 0.5));
    if (i_new_spreaders > mN_Spreaders)
        addSpreaders(i_new_spreaders - mN_Spreaders);

    mN_Spreaders = i_new_spreaders;

}
void SocialNetwork::addSpreaders(uint64_t node_spreaders){
    double add = 0.0;
    for  (uint64_t node = 0; node < node_spreaders; node++){
        bool selected = false;
        do{
            double p =  mUniform(mGen);
            add = mVetProb[0];
            uint64_t j = 1;
            while (p > add)
                add = mVetProb[j++];
            j--;

            if (!mAgents[j].getSpreader()){
                mAgents[j].setSpreader();
                selected = true;
            }//
        }while (!selected);
    }//for  (uint64_t node = 0; node < mSpreaders; node++){
}
/*! \brief Simulation initial condition
 *
 */
void SocialNetwork::initialCondition(void){
    //boost::container::vector <double> prob_vet;
    //boost::container::vector <uint64_t> u_node;
    double K_total = 0.0f, K = 0.0f, add = 0.0f;
    uint64_t source = 0, target = 0;

    boost::adjacency_matrix<boost::directedS> *pGraph;

    mUniform = std::uniform_real_distribution<double>(0.0, 1.0);
    mN_Alpha_Rumor  = std::normal_distribution<double>(mAlpha_Rumor_Ave, mAlpha_Rumor_DP);
    mN_Gamma_Rumor  = std::normal_distribution<double>(mGamma_Rumor_Ave, mGamma_Rumor_DP);


    std::string line;
    //Creating graph from file
    std::cout << "Loading graph - filename:" << mFileGraph << std::endl;
    std::fstream graphFile;
    graphFile.open(mFileGraph, std::fstream::in);
    assert(graphFile.is_open());
    graphFile >> line;
    if (!graphFile.eof()){
    	if (line.compare("*Vertices") == 0){
    			graphFile >> mN_Nodes;
    			mGraph = make_shared<boost::adjacency_matrix<boost::directedS>> (mN_Nodes);
    			pGraph = mGraph.get();
    			graphFile >> line;
    		}

    	    if (line.compare("*Arcs") == 0){
    			do{
    				graphFile >> source;
    				graphFile >> target;
    				std::cout << source << "->" << target << std::endl;
    				add_edge(source, target, *pGraph);
    			}while(!graphFile.eof());
    	    }


    }

    //Selecting spreaders in according the most connected nodes in in_degree
    assert(posix_memalign(reinterpret_cast<void**>(&mVetProb), ALIGN, mN_Nodes *  sizeof(double)) == 0);
    bzero(mVetProb, mN_Nodes *  sizeof(double));

    K_total = 0.0f, K = 0.0f, add = 0.0f;
    for (uint64_t i = 0; i < mN_Nodes; i++)
        K_total += static_cast<double>(boost::in_degree(i, *pGraph));
    K = static_cast<double>(boost::in_degree(0, *pGraph));
    add = K / K_total;
    mVetProb[0] = add;
    for (uint64_t i = 1; i < mN_Nodes; i++){
        K = static_cast<double>(boost::in_degree(i, *pGraph));
        add += K / K_total;
        mVetProb[i] = add;
    }//

    //creating agents
    mAgents.resize(mN_Nodes);
    int64_t id = 0;
    for (uint64_t i =  0; i < mN_Nodes; i++){
    	//mAgents[i] = Agent();
        mAgents[i].setID(id++);
        mAgents[i].setStates(STATES::SUSCEPTIBLE);
    }//for (unsigned int i = 0; i < mN_Nodes; i++){

    mAveAlpha = mN_Alpha = mAveGamma = mN_Gamma = 0.0;

    addSpreaders(mN_Spreaders);

    //It guarantees that the only agent infected is not spreader one
    for  (uint64_t node = 0; node < mN_Spreaders; node++){
        bool selected = false;
        do{
            double p =  mUniform(mGen);
            add = mVetProb[0];
            uint64_t j = 1;
            while (p > add)
                add = mVetProb[j++];
            j--;

            if (!mAgents[j].getSpreader()){
                mAgents[j].setStates(STATES::INFECTED);
                selected = true;
            }//
        }while (!selected);
    }//for  (uint64_t node = 0; node < mSpreaders; node++){


}//void SocialNetwork::initialCondition(void){

void SocialNetwork::exec(void){
    statistic(0);
    std::cout << "\t+update()             ";
	std::cout <<"\t+U[";
	uint64_t mark = mTimeStep / 10;
	for (uint64_t t = 1; t < mTimeStep; t++){
		update(t);
		statistic(t);
		if (( t % mark ) == 0){
			std::cout << ".";
			std::cout.flush();
		}
	}//for (uint64_t t = 1; t < mTimeStep; t++){
	std::cout << "] [OK]" << std::endl;
}//SocialNetwork::exec(void){


void SocialNetwork::statistic(uint64_t timestep){
	fstream logout;
	struct stat buffer;
	size_t state_statistic[4];
	state_statistic[0] = state_statistic[1] = state_statistic[2] = state_statistic[3] = 0;

	for (unsigned int i = 0; i < mN_Nodes; i++){
		unsigned int opt = mAgents[i].getState();
		assert(opt != ZEROSTATE);
		state_statistic[opt-1]++;
	}


	if (stat(mLogFileStatistic.c_str(), &buffer) == -1){
		logout.open(mLogFileStatistic, fstream::trunc|fstream::out);
		assert(logout.is_open());
		logout << "time;susceptible;exposed;infected;recovered;ave_alpha;num_alpha;ave_gamma;num_gamma" << endl;
	}else{
		logout.open(mLogFileStatistic, fstream::app|fstream::out);
		assert(logout.is_open());
	}

    mAveAlpha /= mN_Alpha;
    mAveGamma /= mN_Gamma;
    if (isnan(mAveAlpha)) mAveAlpha = 0.0;
    if (isnan(mAveGamma)) mAveGamma = 0.0;

	logout << timestep << ";" << state_statistic[0] << ";";
	logout << state_statistic[1] << ";" << state_statistic[2] << ";" << state_statistic[3] << ";" << \
	        mAveAlpha << ";" << mN_Alpha << ";" << mAveGamma << ";" << mN_Gamma  << endl;

	logout.close();
    mAveAlpha = mN_Alpha = mAveGamma = mN_Gamma = 0.0;

}//void SocialNetwork::statistic(void){


void SocialNetwork::update(uint64_t t){

		for (uint64_t i = 0; i < mN_Nodes; i++){
			switch (mAgents[i].getState()){
				case SUSCEPTIBLE:susceptible2NS(i, t);break;
				case EXPOSED: exposed2NS(i, t);break;
				case INFECTED:infected2NS(i, t);break;
				//case RECOVERED:r = 0xFF; g = 0xFF; b = 0x00;	break;
			}//switch (opt){
		}

        createSpreader(t);
		for (uint64_t i = 0; i < mN_Nodes; i++){
			mAgents[i].update();
		}



}//void SocialNetwork::update(void){


double SocialNetwork::buildAlpha(uint64_t node, uint64_t iTime){
        boost::adjacency_matrix<boost::directedS> *pGraph = mGraph.get();
		double s = 0.0, k = 0.0, r = 0.0, alpha = 0.0;
		//boost::container::vector <uint64_t> match, matchS;
		//match.clear();
		//matchS.clear();
		//Quem está conectado a mim ?


        std::pair my_pair = in_edges(node, *pGraph); // Get the range of outgoing edges quem eu estou conetado
        for ( auto it = my_pair.first; it != my_pair.second; ++it) {
            k++;
            uint64_t source = boost::source(*it, *pGraph);

            if (mAgents[source].getSpreader()){
            	//The idea is:spreaders propagates the info two time faster than others
                s+=2.0;
                //matchS.push_back(source);
            }else{
                if (mAgents[source].getState() == INFECTED)
                    s++;
                else if (mAgents[source].getState() ==  RECOVERED)
                	r++;
                //match.push_back(source);
            }


        }//for (auto it = edge_begin; it != edge_end; ++it) {

        double p = mN_Alpha_Rumor(mGen); //mW_Alpha;

        double t = static_cast<double>(iTime);
        if (iTime > 0)	t /= p;
        else
        	t = ERROR  / p;

        if (k > 0.0)
            alpha = (1.0 - exp(-((s/k) + t))) -  (1.0 - exp(-((r/k) + t))); //alpha = (s/k) + (1.0 - exp(-c/k));
        else{
        	//I does not have anyone to influence me
        	//Just time influences me 2025-02-16
			alpha =  1.0 - exp(-t);

        }

        if (alpha < 0.0) alpha = 0.0;
		return alpha;
}
double SocialNetwork::buildGamma(uint64_t node, uint64_t iTime){
	//2025-02-16 find where gamma is required
    double r_t = 0.0,
           i_t = 0.0,
		   s_t = 0.0,
           K   = 0.0;
    boost::adjacency_matrix<boost::directedS> *pGraph = mGraph.get();
    std::pair my_pair = in_edges(node, *pGraph); // Get the range of outgoing edges quem eu estou conetado
    for (auto it = my_pair.first; it != my_pair.second; ++it){
        K++;
        uint64_t source = boost::source(*it, *pGraph);
        if (mAgents[source].getState() == RECOVERED){
            r_t++;
        }else if (mAgents[source].getState() == INFECTED){
            i_t++;
        }else  if (mAgents[source].getSpreader()){
            s_t++;
        }
    }//for (auto it = edge_begin; it != edge_end; ++it) {

    //Observação: 2025-02-16
    //A ideia do viez congnitivo acoplado, pois existem vertices que não são influenciados por nada
    //neste caso, tornam-se evetualmente expostos, mas não trocam de estado.
    double t = static_cast<double>(iTime);

    if (K < 1.0){ //There is not neighbor connected to me. Anyone influences me
    	//Just time influences me 2025-02-16
    	r_t = i_t = s_t = 0.0;

    }else{
    	r_t /= K;
		i_t /= K;
		s_t /= K;
    }




    double p = mN_Gamma_Rumor(mGen);
    t /= p;
    double b = 1.0 - exp(-(s_t + i_t + t));
    if (b < 0.0) b = 0.0;

    //if (isnan(b) || isinf(b)){
    //    b = ERROR;
    //    std::cerr << b << std::endl;
    //    exit(-1);
    //}
    ///std::cerr << "[" << b << "]";

    double gamma = r_t + b;

    if (gamma > 1.0) gamma = 1.0;
    if (gamma < 0.0) gamma = 0.0;
    return gamma;

	//return

}//double SocialNetwork::buldGamma(uint64_t node){


/*
double SocialNetwork::buildGamma(uint64_t node, uint64_t time){
    double di = 0.0, in_k = 0.0;
    boost::adjacency_matrix<boost::directedS> *pGraph = mGraph.get();
    std::pair my_pair = in_edges(node, *pGraph); // Get the range of outgoing edges quem eu estou conetado
    for ( auto it = my_pair.first; it != my_pair.second; ++it) {
        in_k++;
        uint64_t source = boost::source(*it, *pGraph);
        if (mAgents[source].getState() == RECOVERED) di++;


    }//for (auto it = edge_begin; it != edge_end; ++it) {
    //if (!(in_k > 0.0))
     //   std::cerr << in_k << " " << di << std::endl;
    assert(in_k >= 0.0);
    double p1 = di /in_k;
    if (isnan(p1)) p1 = 0.0;
    return (1.0 - exp(-static_cast<double>(time) + p1));
	//return

}//double SocialNetwork::buldGamma(uint64_t node){
*/
void SocialNetwork::susceptible2NS(uint64_t node, uint64_t itime){
	double alpha = buildAlpha(node, itime);
	assert(alpha >= 0.0);
	if (alpha >= 1.0) alpha = 1.0;
	double prob = mUniform(mGen);
	mAveAlpha += alpha;
    mN_Alpha++;

    //Recommended network algorithm - paper based: Modeling public opinion dissemination in a multilayer network with SEIR  model based on real social networks
	/*
	 * Assume that the probability that the platform recommends the same information for different users follows
	 * a normal distribution, that is, V ∼ N(mean, dp), where mean represents mean, dp represents variance.
	 *
	 */


	if (prob < alpha){
		//std::cerr << "nop" << std::endl;
		mAgents[node].setState(INFECTED);
	}else if (prob < mEpsilon *  (1.0 - alpha)){
		//std::cerr << "nop" << std::endl;
		mAgents[node].setState(EXPOSED);
	}else{
		//std::cerr << "nop" << std::endl;
		mAgents[node].setState(mAgents[node].getState()); //Keep the state
	}

}//void SocialNetwork::susceptible2NS(uint64_t node){

void SocialNetwork::exposed2NS(uint64_t node, uint64_t itime){
	double gamma = buildGamma(node, itime);
	assert((gamma >= 0.0) && (gamma <= 1.0));
	double alpha =  buildAlpha(node, itime);
	assert(alpha >= 0.0);
	if (alpha >= 1.0) alpha = 1.0;
    mAveAlpha += alpha;
    mN_Alpha++;

    mAveGamma += gamma;
    mN_Gamma++;


	double prob = mUniform(mGen);
	if (prob < gamma){
			//std::cerr <<  "exposed2NS EXPOSED -> RECOVERED" << std::endl;
			mAgents[node].setState(RECOVERED);
	}else if (prob < alpha * (1.0 - gamma)){
			//std::cerr <<  "exposed2NS EXPOSED -> INFECTED" << std::endl;
			mAgents[node].setState(INFECTED);
	}else{
		//std::cerr <<  "exposed2NS keeps state" << std::endl;
		mAgents[node].setState(mAgents[node].getState()); //Keep the state
	}



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
    boost::container::vector <uint64_t> in_hist, out_hist;
    uint64_t iK_in_max = 0, iK_out_max = 0;
    boost::adjacency_matrix<boost::directedS> *pGraph = mGraph.get();
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


}//void SocialNetwork::printNetworkStatistic(void){


void SocialNetwork::print(void){
	bool flag = mN_Nodes == (mCellX * mCellY);
    cout << endl << "Simulation of online information" << endl;
	cout << " - Config:" << endl;
	cout << "\tNodes(" << mN_Nodes << ") = " << mCellX << "x" <<  mCellY << " = ["  << std::boolalpha << flag << "] | Timesteps(" << mTimeStep << ")" << endl;
	//cout << "\tThe average degree: " << setw(8) << fixed << setprecision(4) << mAve_degree  << endl;
	//cout << "\tThe maximum degree: " << setw(8) << fixed << setprecision(4) << mMax_degree  << endl;
	cout << "\tEpsion rate       : (" << setw(8) << fixed << setprecision(4) << mEpsilon  << ")" << endl;
	cout << "\tAlpha Normal      : (" << setw(8) << fixed << setprecision(4) << mAlpha_Rumor_Ave  << "," << mAlpha_Rumor_DP << ")" << endl;
	cout << "\tGamma Normal      : (" << setw(8) << fixed << setprecision(4) << mGamma_Rumor_Ave  << "," << mGamma_Rumor_DP << ")" << endl;
	cout << "\tSpreaders         : (" << setw(8) << fixed << mN_Spreaders << ")" << endl;
	cout << "\tLog files: " << endl;
	cout << "\t - Log file statistic:      [" << mLogFileStatistic << "]" << endl;
	cout << "\t - State file state agents: [" << mFileStateAgent << "]" <<endl;
	//cout << "\t - State file graph:        [" << mStateFileGraph << "]" << endl;
	cout << "\t - File graph info:         [" << mFileGraph << "]" << endl << endl << endl;
}
