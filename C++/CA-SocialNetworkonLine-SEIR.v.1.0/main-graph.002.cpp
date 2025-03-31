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
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_matrix.hpp>

#include <iomanip>
using namespace boost::container;
using namespace std;
//void Scale_free_network();
enum { A, B, C, D, E, F, N };
const char* name = "ABCDEF";
//typedef boost::adjacency_matrix<boost::directedS> Graph;
int main (int ac, char **av){
	std::cout << std::endl << "Scale-free-network builder" << std::endl;
	boost::adjacency_matrix<boost::directedS> graph(N);

    add_edge(B, C, graph);
    add_edge(B, F, graph);
    add_edge(C, A, graph);
    add_edge(C, C, graph);
    add_edge(D, E, graph);
    add_edge(E, D, graph);
    add_edge(F, A, graph);


    std::pair<boost::graph_traits<boost::adjacency_matrix<boost::directedS>>::in_edge_iterator, boost::graph_traits<boost::adjacency_matrix<boost::directedS>>::in_edge_iterator> my_pair  = in_edges(0, graph); // Get the range of outgoing edges quem eu estou conetado

    std::cout << "\nArestas do grafo:" << std::endl;
    auto [edge_begin, edge_end] = boost::edges(graph); // Obtém iteradores para as arestas
    for (auto it = edge_begin; it != edge_end; ++it) {
        auto source = boost::source(*it, graph); // Vértice de origem
        auto target = boost::target(*it, graph); // Vértice de destino
        std::cout << source << " -> " << target << std::endl;
    }
    std::cout << "Grau de cada vértice:" << std::endl;
    auto [vertex_begin, vertex_end] = boost::vertices(graph);
    for (auto it = vertex_begin; it != vertex_end; ++it) {
        std::cout << "Vértice " << *it << ": grau entrada " << boost::in_degree(*it, graph) << std::endl;
        std::cout << "Vértice " << *it << ": grau saída " << boost::out_degree(*it, graph) << std::endl;
    }


    for (auto it = vertex_begin; it != vertex_end; ++it) {
        std::cout << "Vizinhos do vértice " << *it << ": ";

        // Obtém os vértices adjacentes ao vértice atual
        auto [neighbor_begin, neighbor_end] = boost::adjacent_vertices(*it, graph);
        for (auto neighbor = neighbor_begin; neighbor != neighbor_end; ++neighbor) {
            std::cout << *neighbor << " ";
        }
        std::cout << std::endl;
    }

    int u = 5, v = 0; // vertices to check
    auto result = edge(u, v, graph);

    if (result.second) {
        std::cout << "Edge exists between " << u << " and " << v << std::endl;
    } else {
        std::cout << "No edge exists between " << u << " and " << v << std::endl;
    }
	return EXIT_SUCCESS;
}





