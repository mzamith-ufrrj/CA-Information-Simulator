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

int main (int ac, char **av){
    typedef boost::adjacency_matrix<boost::directedS> Graph;
    Graph g(5); // Create a graph with 5 vertices (0 to 4)

    // Add edges (source, target)
    add_edge(0, 1, g);
    add_edge(0, 2, g);
    add_edge(1, 3, g);
    add_edge(3, 4, g);

    // Vertex from which to find the connected vertices
    for (int vertex = 0; vertex < 5; vertex++){
        // Iterate over outgoing edges of the vertex
        auto [ei, ei_end] = in_edges(vertex, g); // Get the range of outgoing edges
        std::cout << "Vertices connected to vertex " << vertex << " <- : ";
        for (; ei != ei_end; ++ei) {
            std::cout << target(*ei, g) << " "; // Get the target vertex of the edge
        }
        std::cout << std::endl;
    }





	return EXIT_SUCCESS;
}





