#include <fstream>
#include <chrono>
#include <random>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <boost/container/vector.hpp>
#include <boost/range/algorithm/find.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <iomanip>
using namespace boost::container;
using namespace std;

void Scale_free_network(boost::adjacency_matrix<boost::directedS> &graph,
                        uint64_t num_nodes, uint64_t m){
    boost::container::vector <double> in_prob_vet, out_prob_vet;
    boost::container::vector <uint64_t> u_node;
    std::default_random_engine gen;
    std::uniform_real_distribution<double> uniform(0,1);
    double K_total = 0.0f, K = 0.0f, add = 0.0f;


    gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
    //Part 1:
    std::cout << "Building K(" << m << ") relation" << std::endl;
    std::cout.flush();
    for (uint64_t i = 0; i < m; i++){
        for (uint64_t j = i+1; j < m; j++){
            //std::cout << "Source: " << i <<  " target: " << j << std::endl;
            add_edge(i, j, graph);
            add_edge(j, i, graph);
        }//for (uint64_t j = i+1; j < m; j++){
    }//for (uint64_t i = 0; i < m; i++){
    /*
    std::cout << "Showing the graph" << std::endl;
    for (uint64_t i = 0; i < m; i++){
        cout << "(" << i << ") -> ";
        auto [neighbor_begin, neighbor_end] = boost::adjacent_vertices(i, graph);
        for (auto neighbor = neighbor_begin; neighbor != neighbor_end; ++neighbor) {
            std::cout << *neighbor << " ";
        }
        std::cout << std::endl;
    }//for (uint64_t i = 0; i < m; i++){
    */
    //Part 2:

    uint64_t iMod = num_nodes / 10;
    std::cout << std::endl << " -| "; std::cout.flush();
    for (uint64_t node = m; node < num_nodes; node++){

        if ((node % iMod) == 0){
            std::cout << "."; std::cout.flush();
        }
        in_prob_vet.resize(node);

        K_total = 0.0f, K = 0.0f, add = 0.0f;
        for (uint64_t i = 0; i < node; i++)
            K_total += static_cast<double>(boost::in_degree(i, graph));

        K = static_cast<double>(boost::in_degree(0, graph));
        add = K / K_total;
        in_prob_vet[0] = add;

        for (uint64_t i = 1; i < node; i++){
            K = static_cast<double>(boost::in_degree(i, graph));
            add += K / K_total;
            in_prob_vet[i] = add;
        }//        for (uint64_t i = 0; i < node; i++){

        //Part 3 - selecting m unique nodes in order to connect one of them
        u_node.clear();
        for (uint64_t i = 0; i < m; i++){
            bool selected = false;
            do{
                double p =  uniform(gen);
                add = in_prob_vet[0];
                uint64_t j = 1;
                while (p > add)
                    add = in_prob_vet[j++];
                j--;
                auto ret = boost::find(u_node, j);
                if (ret == u_node.end()){//not found
                    u_node.push_back(j);
                    add_edge(node, j, graph);
                    selected  = true;
                }//if (ret == u_node.end()){//not found
            }while(!selected);


        }//for (uint64_t i = 0; i < m; i++){
        //Part 4 - selecting the most likely node out_degree
        //-------------------------------------------------------------
        out_prob_vet.resize(node);

        K_total = 0.0f, K = 0.0f, add = 0.0f;
        for (uint64_t i = 0; i < node; i++)
            K_total += static_cast<double>(boost::out_degree(i, graph));

        K = static_cast<double>(boost::out_degree(0, graph));
        add = K / K_total;
        out_prob_vet[0] = add;
        for (uint64_t i = 1; i < node; i++){
            K = static_cast<double>(boost::out_degree(i, graph));
            add += K / K_total;
            out_prob_vet[i] = add;
        }//

        u_node.clear();
        for (uint64_t i = 0; i < m; i++){
            bool selected = false;
            uint64_t out_node;
            do{
                double p =  uniform(gen);
                add = out_prob_vet[0];
                uint64_t j = 1;
                while (p > add)
                    add = out_prob_vet[j++];
                j--;
                auto ret = boost::find(u_node, j);
                if (ret == u_node.end()){//not found
                    u_node.push_back(j);
                    add_edge(j, node, graph);
                    out_node = j;
                    selected  = true;
                }//if (ret == u_node.end()){//not found
            }while(!selected);
            //uint64_t iK = static_cast<uint64_t>(boost::out_degree(node, graph));
        }//for (uint64_t i = 0; i < m; i++){
        //--------------------------------------------------------------

    }//for (uint64_t node = m; node < num_nodes; node++){
    std::cout << std::endl;
}//void Scale_free_network(boost::adjacency_matrix<boost::directedS> &graph,
int main (int ac, char **av){
	std::cout << std::endl << "Scale-free-network builder" << std::endl;
	std::cout << std::endl << sizeof(uint64_t) << std::endl;
	uint64_t num_nodes = std::stoi(av[1]);
	uint64_t m = std::stoi(av[2]);;
	std::string filename = std::string(av[3]);
	std::string s_log_in = filename + ".in_degree";
    std::string s_log_out = filename + ".out_degree";
    std::cout << "num_nodes : " << num_nodes << std::endl;
    std::cout << "        m : " << m << std::endl;
    std::cout << "in_degree file  : " << s_log_in << std::endl;
    std::cout << "out_degree file : " << s_log_out << std::endl;


	boost::adjacency_matrix<boost::directedS> graph(num_nodes);

    Scale_free_network(graph, num_nodes, m);

    boost::container::vector <uint64_t> in_hist, out_hist;
    std::cout << "Building histogram in degree" << std::endl;
    uint64_t iK_in_max = 0, iK_out_max = 0;
    for (uint64_t i = 0; i < num_nodes; i++){
        uint64_t iK = static_cast<uint64_t>(boost::in_degree(i, graph));
        if (iK > iK_in_max) iK_in_max = iK;

        iK = static_cast<uint64_t>(boost::out_degree(i, graph));
        if (iK > iK_out_max) iK_out_max = iK;
        //std::cout << "Node(" << i << ") -> K = " << iK << std::endl;
    }//
    std::cout << "Max  in K = " << iK_in_max << std::endl;
    std::cout << "Max out K = " << iK_out_max << std::endl;
    in_hist.resize(iK_in_max);
    out_hist.resize(iK_out_max);
    for (uint64_t i = 0; i < num_nodes; i++){
        uint64_t iK = static_cast<uint64_t>(boost::in_degree(i, graph));
        in_hist[iK-1]++;
        iK = static_cast<uint64_t>(boost::out_degree(i, graph));
        out_hist[iK-1]++;
    }


    std::fstream output;
    output.open(s_log_in, std::fstream::out|std::fstream::trunc);
	assert(output.is_open());
	for (uint64_t n = 0; n < in_hist.size(); n++)
		output << n+1 << " " << in_hist[n] << std::endl;
	output.close();

	output.open(s_log_out, std::fstream::out|std::fstream::trunc);
	assert(output.is_open());
	for (uint64_t n = 0; n < out_hist.size(); n++)
		output << n+1 << " " << out_hist[n] << std::endl;
	output.close();


    //saving
    std::string gfileName = "graph.bin" ;
    std::cout << "Saving graph - filename:" << gfileName << std::endl;
    std::fstream graphFile;
    graphFile.open(gfileName, std::fstream::out|std::fstream::trunc | std::fstream::binary);
    assert(graphFile.is_open());
    graphFile.write(reinterpret_cast<char *>(&num_nodes), sizeof(uint64_t));
    auto [edge_begin, edge_end] = boost::edges(graph); // Obtém iteradores para as arestas
    for (auto it = edge_begin; it != edge_end; ++it) {
        uint64_t source = reinterpret_cast<uint64_t>(boost::source(*it, graph)); // Vértice de origem
        uint64_t target = reinterpret_cast<uint64_t>(boost::target(*it, graph)); // Vértice de destino
        graphFile.write(reinterpret_cast<char *>(&source), sizeof(uint64_t));
        graphFile.write(reinterpret_cast<char *>(&target), sizeof(uint64_t));

    }
    graphFile.close();

    /*
    for (auto it = edge_begin; it != edge_end; ++it) {
        auto source = boost::source(*it, graph); // Vértice de origem
        auto target = boost::target(*it, graph); // Vértice de destino
        std::cout << source << " -> " << target << std::endl;
    }
    */

	return EXIT_SUCCESS;
}





