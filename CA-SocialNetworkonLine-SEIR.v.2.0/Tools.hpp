#ifndef _TOOLS_HPP_
#define _TOOLS_HPP_
#include <Agent.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <iomanip>
#define ERROR 1E-20
enum STATES{ZEROSTATE, SUSCEPTIBLE, EXPOSED, INFECTED, RECOVERED};
void Scale_free_network(boost::adjacency_matrix<boost::directedS> &graph,
                        uint64_t num_nodes, uint64_t m);

void Scale_free_network(boost::adjacency_matrix<boost::directedS> &graph,
                        uint64_t num_nodes, double alpha_in, double alpha_out);

void Scale_free_network_v2_0(boost::adjacency_matrix<boost::directedS> &graph,
                        uint64_t num_nodes, const double alpha_in, const double alpha_out);
#endif
