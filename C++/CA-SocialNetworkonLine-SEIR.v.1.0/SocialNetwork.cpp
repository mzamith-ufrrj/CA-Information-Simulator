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
		mTimeStep(0), mVerbose(true),  mAve_in_degree(0.0), mAve_out_degree(0.0),
		mMax_in_degree(0.0), mMax_out_degree(0.0), mK_Value(0.0),
		mEpsilon(0.0), mAlpha(0.0), mGamma(0.0), mX_Nodes(0), mY_Nodes(0){
    mGen.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

SocialNetwork::~SocialNetwork(){}

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
    //mLogFileGraph   = std::string("diagram." + name + ".ppm");
    //mStateFileGraph   = std::string("graph." + name + ".binary");
    mFileGraph   = std::string("graph." + name );

    mN_Nodes     = root["global-params"].get("nodes", "UTF-8" ).asUInt();
    mX_Nodes     = root["global-params"].get("nodes_x", "UTF-8" ).asUInt();
    mY_Nodes     = root["global-params"].get("nodes_y", "UTF-8" ).asUInt();
    mN_Spreaders = root["global-params"].get("spreaders", "UTF-8" ).asUInt();
    mTimeStep    = root["global-params"].get("timesteps", "UTF-8" ).asUInt();

    mEpsilon       = root["global-params"].get("epsilon", "UTF-8" ).asDouble();
    mK_Value       = root["global-params"].get("K-value", "UTF-8" ).asDouble();
    assert(mN_Nodes == (mX_Nodes * mY_Nodes));


}

/*! \brief Simulation initial condition
 *
 */
void SocialNetwork::initialCondition(void){
    mUniform = std::uniform_real_distribution<double>(0.0, 1.0);
    mAgents.resize(mN_Nodes);

    mGraph = make_shared<boost::adjacency_matrix<boost::directedS>> (mN_Nodes);

    boost::adjacency_matrix<boost::directedS> *pGraph = mGraph.get();
    //Building scale free network
    boost::container::vector <double> in_prob_vet, out_prob_vet;
    boost::container::vector <uint64_t> u_node;
    double K_total = 0.0f, K = 0.0f, add = 0.0f;
    uint64_t iK_Value = static_cast<uint64_t>(mK_Value);


    //Initial relation
    for (uint64_t i = 0; i < iK_Value; i++){
        for (uint64_t j = i+1; j < iK_Value; j++){
            //std::cout << "Source: " << i <<  " target: " << j << std::endl;
            add_edge(i, j, *pGraph);
            add_edge(j, i, *pGraph);
        }//for (uint64_t j = i+1; j < m; j++){
    }//for (uint64_t i = 0; i < m; i++){
    uint64_t iMod = mN_Nodes / 10;
    for (uint64_t node = iK_Value; node < mN_Nodes; node++){
        if (mVerbose){
           if ((( (node + 1) % iMod) == 0) || (node == mN_Nodes - 1)){
                std::cout << "Nodes: " <<   std::setw(8) <<  std::fixed << (node + 1) << " inserted" << std::endl; std::cout.flush();
           }//if ((node % iMod) == 0){

        }//if (mVerbose){
        in_prob_vet.resize(node);
        K_total = 0.0f, K = 0.0f, add = 0.0f;
        //Building probabilities based on vertice in degree
        for (uint64_t i = 0; i < node; i++)
            K_total += static_cast<double>(boost::in_degree(i, *pGraph));
        K = static_cast<double>(boost::in_degree(0, *pGraph));
        add = K / K_total;
        in_prob_vet[0] = add;
        for (uint64_t i = 1; i < node; i++){
            K = static_cast<double>(boost::in_degree(i, *pGraph));
            add += K / K_total;
            in_prob_vet[i] = add;
        }//        for (uint64_t i = 0; i < node; i++){

        u_node.clear();
        //Selecting probabilities based on vertice in degree
        for (uint64_t i = 0; i < iK_Value; i++){
            bool selected = false;
            do{
                double p =  mUniform(mGen);
                add = in_prob_vet[0];
                uint64_t j = 1;
                while (p > add)
                    add = in_prob_vet[j++];
                j--;
                auto ret = boost::find(u_node, j);
                if (ret == u_node.end()){//not found
                    u_node.push_back(j);
                    add_edge(node, j, *pGraph);
                    selected  = true;
                }//if (ret == u_node.end()){//not found
            }while(!selected);

        }// for (uint64_t i = 0; i < iK_Value; i++){
        //Building probabilities based on vertice out degree
        out_prob_vet.resize(node);
        K_total = 0.0f, K = 0.0f, add = 0.0f;
        for (uint64_t i = 0; i < node; i++)
            K_total += static_cast<double>(boost::out_degree(i, *pGraph));
        K = static_cast<double>(boost::out_degree(0, *pGraph));
        add = K / K_total;
        out_prob_vet[0] = add;
        for (uint64_t i = 1; i < node; i++){
            K = static_cast<double>(boost::out_degree(i, *pGraph));
            add += K / K_total;
            out_prob_vet[i] = add;
        }//

        u_node.clear();
        for (uint64_t i = 0; i < iK_Value; i++){
            bool selected = false;

            do{
                double p =  mUniform(mGen);
                add = out_prob_vet[0];
                uint64_t j = 1;
                while (p > add)
                    add = out_prob_vet[j++];
                j--;
                auto ret = boost::find(u_node, j);
                if (ret == u_node.end()){//not found
                    u_node.push_back(j);
                    add_edge(j, node, *pGraph);

                    selected  = true;
                }//if (ret == u_node.end()){//not found
            }while(!selected);
            //uint64_t iK = static_cast<uint64_t>(boost::out_degree(node, graph));
        }//for (uint64_t i = 0; i < iK_Value; i++){
    }//for (uint64_t node = m; node < num_nodes; node++){


    //Creating Agents and spreaders
    int64_t id = 0;
    for (uint64_t i =  0; i < mN_Nodes; i++){
    	//mAgents[i] = Agent();
        mAgents[i].setID(id++);
        mAgents[i].setStates(STATES::SUSCEPTIBLE);
    }//for (unsigned int i = 0; i < mN_Nodes; i++){
    //mGraph = boost::adjacency_matrix<boost::directedS>(mN_Nodes);
    /// ---> change it to select the most in_degree agents

    //Selecting spreaders in according the most connected nodes in in_degree
    in_prob_vet.resize(mN_Nodes);
    K_total = 0.0f, K = 0.0f, add = 0.0f;
    for (uint64_t i = 0; i < mN_Nodes; i++)
        K_total += static_cast<double>(boost::in_degree(i, *pGraph));
    K = static_cast<double>(boost::in_degree(0, *pGraph));
    add = K / K_total;
    in_prob_vet[0] = add;
    for (uint64_t i = 1; i < mN_Nodes; i++){
        K = static_cast<double>(boost::in_degree(i, *pGraph));
        add += K / K_total;
        in_prob_vet[i] = add;
    }//

    //The mN_Spreaders spreaders most influence of the network
	mSpreaders.clear();
	for (uint64_t i = 0; i < mN_Spreaders; i++){
		bool selected = false;

        do{
            double p =  mUniform(mGen);
            add = in_prob_vet[0];
            uint64_t j = 1;
            while (p > add)
                add = in_prob_vet[j++];
            j--;
            auto ret = boost::find(mSpreaders, j);
            if (ret == mSpreaders.end()){//not found
                mSpreaders.push_back(j);
                selected  = true;
            }//if (ret == u_node.end()){//not found
        }while(!selected);
		//uint64_t k = static_cast<uint64_t>(mUniform(mGen) * static_cast<double>(mN_Nodes));
		//mSpreaders.push_back(k);
	}//for (uint64_t i = 0; i < mN_Spreaders; i++){

    in_prob_vet.resize(mSpreaders.size());
    K_total = 0.0f, K = 0.0f, add = 0.0f;
    for (uint64_t i = 0; i < mSpreaders.size(); i++){
        uint64_t vertice = mSpreaders[i];
        K_total += static_cast<double>(boost::in_degree(vertice, *pGraph));
    }

    K = static_cast<double>(boost::in_degree(mSpreaders[0], *pGraph));
    add = K / K_total;
    in_prob_vet[0] = add;
    for (uint64_t i = 1; i < mSpreaders.size(); i++){
        K = static_cast<double>(boost::in_degree(mSpreaders[i], *pGraph));
        add += K / K_total;
        in_prob_vet[i] = add;
    }//
    double p =  mUniform(mGen);
    add = in_prob_vet[0];
    uint64_t j = 1;
    while (p > add)
        add = in_prob_vet[j++];
    j--;
    uint64_t vertice = mSpreaders[j];
    mAgents[vertice].setStates(STATES::INFECTED);
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
		logout << "time;susceptible;exposed;infected;recovered" << endl;
	}else{
		logout.open(mLogFileStatistic, fstream::app|fstream::out);
		assert(logout.is_open());
	}

	logout << timestep << ";" << state_statistic[0] << ";";
	logout << state_statistic[1] << ";" << state_statistic[2] << ";" << state_statistic[3] << endl;

	logout.close();


}//void SocialNetwork::statistic(void){


void SocialNetwork::update(uint64_t t){

		for (uint64_t i = 0; i < mN_Nodes; i++){
			switch (mAgents[i].getState()){
				case SUSCEPTIBLE:susceptible2NS(i);break;
				case EXPOSED: exposed2NS(i, t);break;
				case INFECTED:infected2NS(i, t);break;
				//case RECOVERED:r = 0xFF; g = 0xFF; b = 0x00;	break;
			}//switch (opt){
		}

		for (uint64_t i = 0; i < mN_Nodes; i++){
			mAgents[i].update();
		}

}//void SocialNetwork::update(void){


double SocialNetwork::buildAlpha(uint64_t node){
        boost::adjacency_matrix<boost::directedS> *pGraph = mGraph.get();
		double s = 0.0, k = 0.0, c = 0.0;
		boost::container::vector <uint64_t> match, matchS;
		match.clear();
		matchS.clear();
		//Quem está conectado a mim ?
        // auto [ei, ei_end] = out_edges(vertex, g); // Get the range of outgoing edges quem eu estou conetado

		/*
		for (uint64_t i = 0; i < mN_Nodes; i++){
			if (i != node){
				bool ret = mAdjMatrix[node * mN_Nodes + i];
				if (ret){
					k++;
					auto retS = boost::find(mSpreaders, i);
					if (retS != mSpreaders.end()){
						s++;
						matchS.push_back(i);
					}//if (retS != neighbors->end()){
					else
						match.push_back(i);
				}//if (ret){
			}//if (i != node){
		}//for (uint64_t i = 0; i < mN_Nodes; i++){
        */
		//Obtaining C parameter of equation
		for (uint64_t i = 0; i < matchS.size(); i++){
			uint64_t i_s =  matchS[i];
			for (uint64_t j = 0; j < match.size(); j++){
				uint64_t j_n = match[j];
				if (mAdjMatrix[j_n * mN_Nodes + i_s]){
					c++;
				}//if (mAdjMatrix[j_n * mN_Nodes + i_s]){
			}//for (uint64_t j = 0; j < match.size(); j++){
		}//for (uint64_t i = 0; i < matchS.size(); i++){

		return (s/k) + (1.0 - exp(-c/k));

		return 0.0;
}
double SocialNetwork::buldGamma(uint64_t node, uint64_t time){
/*
	double di = 0.0, in_k = 0.0;
	for (uint64_t i = 0; i < mN_Nodes; i++){
		if (i != node){
			bool ret = mAdjMatrix[i * mN_Nodes + node];
			if (ret){
				if (mAgents[i].getState() == RECOVERED) di++;
				in_k++;
			}//if (ret){
		}//if (i != node){
	}//for (uint64_t i = 0; i < mN_Nodes; i++){
	double gamma = 1.0 - exp(-((di+1)/in_k) *  static_cast<double>(time));

	return gamma;*/
	return 0.0;

}//double SocialNetwork::buldGamma(uint64_t node){

void SocialNetwork::susceptible2NS(uint64_t node){
	double alpha = buildAlpha(node);
	assert(alpha >= 0.0);
	if (alpha >= 1.0) alpha = 1.0;
	double prob = mUniform(mGen);


	if (prob < alpha)
		mAgents[node].setState(INFECTED);
	else if (prob < mEpsilon * (1.0 - alpha))
		mAgents[node].setState(EXPOSED);
	else
		mAgents[node].setState(mAgents[node].getState()); //Keep the state

}//void SocialNetwork::susceptible2NS(uint64_t node){

void SocialNetwork::exposed2NS(uint64_t node, uint64_t time){
/*
	double gamma = buldGamma(node, time);
	double alpha =  buildAlpha(node);
	assert(alpha >= 0.0);
	if (alpha >= 1.0) alpha = 1.0;


	assert((gamma >= 0.0) && (gamma <= 1.0));
	double prob = mUniform(mGen);

	if (prob < gamma)
		mAgents[node].setState(RECOVERED);
	else if (prob < alpha * (1.0 - gamma))
		mAgents[node].setState(INFECTED);
	else
		mAgents[node].setState(mAgents[node].getState()); //Keep the state
		*/
}

void SocialNetwork::infected2NS(uint64_t node, uint64_t time){
	/*
	double gamma = buldGamma(node, time);

	assert((gamma >= 0.0) && (gamma <= 1.0));
	double prob = mUniform(mGen);

	if (prob < gamma) mAgents[node].setState(RECOVERED);
	else mAgents[node].setState(mAgents[node].getState()); //Keep the state
	*/
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
    in_hist.resize(iK_in_max);
    out_hist.resize(iK_out_max);
    for (uint64_t i = 0; i < mN_Nodes; i++){
        uint64_t iK = static_cast<uint64_t>(boost::in_degree(i, *pGraph));
        in_hist[iK-1]++;
        iK = static_cast<uint64_t>(boost::out_degree(i, *pGraph));
        out_hist[iK-1]++;
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
    cout << endl << "Simulation of online information" << endl;
	cout << " - Config:" << endl;
	cout << "\tNodes(" << mN_Nodes << " -> [" << mX_Nodes << "," << mY_Nodes << "]) | Timesteps(" << mTimeStep << ")" << endl;
	//cout << "\tThe average degree: " << setw(8) << fixed << setprecision(4) << mAve_degree  << endl;
	//cout << "\tThe maximum degree: " << setw(8) << fixed << setprecision(4) << mMax_degree  << endl;
	cout << "\tEpsion rate       : " << setw(8) << fixed << setprecision(4) << mEpsilon  << endl;
	cout << "\tSpreaders         : " << setw(8) << fixed << mN_Spreaders << endl;
	cout << "\tLog files: " << endl;
	cout << "\t - Log file statistic:      [" << mLogFileStatistic << "]" << endl;
	cout << "\t - State file state agents: [" << mFileStateAgent << "]" <<endl;
	//cout << "\t - State file graph:        [" << mStateFileGraph << "]" << endl;
	cout << "\t - File graph info:         [" << mFileGraph << ".[in/out]_degree/topology]" << endl;
}