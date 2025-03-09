#include <fstream>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <Tools.hpp>
#include <boost/container/vector.hpp>
#include <boost/graph/adjacency_matrix.hpp>
using namespace boost::container;
/*
 * ./build-free-scale-network <true = 1/false = 0> <label> <number of nodes> [m -> uint64 | alpha_in - double] [ alpha_out]
 *   - <true/false>: true runs slower version or flasher version
 *   - <label> is file label: graph, in_degree and out_degree
 *   - if <true> we give m parameter otherwise alpha_in and alpha_out
 *   ./build-free-scale-network 0 teste-001000 1000 2 2
 */
int main (int ac, char **av){
	std::string sType      = "",
	            filename   = "",
	            s_log_in   = "",
	            s_log_out  = "",
	            s_graph    = "";

	uint64_t num_nodes = std::stoi(av[3]),
	         m = 0,
	         iK_in_max  = 0,
	         iK_out_max = 0;

	double alpha_in  = 0.0,
	       alpha_out = 0.0,
	       gamma_in  = 0.0,      //(1.0 + alpha_in) / alpha_in;
	       gamma_out = 0.0,    // (1.0 + alpha_out) / alpha_out;
	       ave_in = 0.0,
	       ave_out = 0.0;

    boost::container::vector <uint64_t> in_hist,
                                        out_hist;

    filename = av[2];
	  bool type = std::stoi(av[1]);
    if (type){
        sType = "\t - Slower version";
        m = std::stoi(av[4]);
    }else{
        sType = "\t - Faster version";
        alpha_in =  std::stod(av[4]);
        alpha_out =  std::stod(av[5]);
        gamma_in = (1.0 + alpha_in) / alpha_in;
        gamma_out = (1.0 + alpha_out) / alpha_out;
    }



	s_log_in  = filename + ".in_degree.csv";
    s_log_out = filename + ".out_degree.csv";
    s_graph   = filename + ".graph.bin";

    std::cout << std::endl << "Scale-free-network builder" << std::endl;
	std::cout << std::endl << sizeof(uint64_t) << std::endl;
	std::cout << "num_nodes : " << num_nodes << std::endl;
	if (type)
        std::cout << "        m : " << m << std::endl;
    std::cout << "in_degree file  : " << s_log_in << std::endl;
    std::cout << "out_degree file : " << s_log_out << std::endl;
    std::cout << "graph file      : " << s_graph << std::endl;
    if (!type){
        std::cout << " in alpha: " << alpha_in  << " /  in gamma: " << gamma_in << std::endl;
        std::cout << "out alpha: " << alpha_out << " / out gamma: " << gamma_out << std::endl;
    }

    std::unique_ptr<boost::adjacency_matrix<boost::directedS>> mGraph;
    mGraph = std::make_unique<boost::adjacency_matrix<boost::directedS>> (num_nodes);
    boost::adjacency_matrix<boost::directedS> *ptr = mGraph.get();

	//boost::adjacency_matrix<boost::directedS> *graph;
	//boost::adjacency_matrix<boost::directedS> graph(num_nodes);
	if (type)
        Scale_free_network(*ptr, num_nodes, m);
    else
        Scale_free_network_v2_0(*ptr, num_nodes, alpha_in, alpha_out);


    std::cout << "Building histogram in degree" << std::endl;
    ave_in = ave_out = 0.0;

    for (uint64_t i = 0; i < num_nodes; i++){
        uint64_t iK = static_cast<uint64_t>(boost::in_degree(i, *ptr));
        ave_in += static_cast<double>(iK);
        if (iK > iK_in_max) iK_in_max = iK;

        iK = static_cast<uint64_t>(boost::out_degree(i, *ptr));
        ave_out += static_cast<double>(iK);
        if (iK > iK_out_max) iK_out_max = iK;
        //std::cout << "Node(" << i << ") -> K = " << iK << std::endl;
    }//
    ave_in /= static_cast<double>(num_nodes);
    ave_out /= static_cast<double>(num_nodes);

    std::cout << "Maximum degree:" << std::endl;
    std::cout << "\t - in  K: " << iK_in_max << std::endl;
    std::cout << "\t - out K: " << iK_out_max << std::endl;


    std::cout << "Average:" << std::endl;
    std::cout << "\t - in  K: " << std::setw(10) << std::fixed << std::setprecision(4) << ave_in << std::endl;
    std::cout << "\t - out K: " <<  std::setw(10) << std::fixed << std::setprecision(4) << ave_out << std::endl;


    /*
    std::cout << "Vertice degree:" << std::endl;
    auto [vertex_begin, vertex_end] = boost::vertices(*ptr);
    for (auto it = vertex_begin; it != vertex_end; ++it) {
        std::cout << " - in  [vertice: " << *it << "]  in_degree: " << boost::in_degree(*it, *ptr) << std::endl;
        std::cout << " - out [vertice: " << *it << "] out_degree: " << boost::out_degree(*it, *ptr) << std::endl;
    }


    for (auto it = vertex_begin; it != vertex_end; ++it) {
        std::cout << "Vertice [" << *it << "] neighbors: ";

        // Obtém os vértices adjacentes ao vértice atual
        auto [neighbor_begin, neighbor_end] = boost::adjacent_vertices(*it, *ptr);
        for (auto neighbor = neighbor_begin; neighbor != neighbor_end; ++neighbor) {
            std::cout << *neighbor << " ";
        }
        std::cout << std::endl;
    }
    */
    in_hist.resize(iK_in_max+1);
    out_hist.resize(iK_out_max+1);

    for (uint64_t i = 0; i < num_nodes; i++){
        uint64_t iK = static_cast<uint64_t>(boost::in_degree(i, *ptr));
        in_hist[iK]++;
        iK = static_cast<uint64_t>(boost::out_degree(i, *ptr));
        out_hist[iK]++;
    }
    std::cout  << "Zero info: " << std::endl;
    std::cout  << "\t -  in: " << in_hist[0] << std::endl;
    std::cout  << "\t - out: " << out_hist[0] << std::endl;

    std::fstream output;
    output.open(s_log_in, std::fstream::out|std::fstream::trunc);
	assert(output.is_open());
	output << "degree;frequency" << std::endl;
	for (uint64_t n = 0; n < in_hist.size(); n++)
		output << n+1 << ";" << in_hist[n] << std::endl;
	output.close();

	output.open(s_log_out, std::fstream::out|std::fstream::trunc);
	assert(output.is_open());
	output << "degree;frequency" << std::endl;
	for (uint64_t n = 0; n < out_hist.size(); n++)
		output << n+1 << ";" << out_hist[n] << std::endl;
	output.close();


    //saving

    std::cout << "Saving graph - filename:" << s_graph << std::endl;
    std::fstream graphFile;
    graphFile.open(s_graph, std::fstream::out|std::fstream::trunc | std::fstream::binary);
    assert(graphFile.is_open());
    graphFile.write(reinterpret_cast<char *>(&num_nodes), sizeof(uint64_t));
    auto [edge_begin, edge_end] = boost::edges(*ptr); // Obtém iteradores para as arestas
    for (auto it = edge_begin; it != edge_end; ++it) {
        uint64_t source = reinterpret_cast<uint64_t>(boost::source(*it, *ptr)); // Vértice de origem
        uint64_t target = reinterpret_cast<uint64_t>(boost::target(*it, *ptr)); // Vértice de destino
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





