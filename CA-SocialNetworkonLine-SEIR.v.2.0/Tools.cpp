#include <fstream>
#include <chrono>
#include <random>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <boost/container/vector.hpp>
#include <boost/range/algorithm/find.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/adjacency_matrix.hpp>

using namespace boost::container;
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

    //Part 2:
    uint64_t iMod = num_nodes / 10;

    for (uint64_t node = m; node < num_nodes; node++){

        if (((node+1) % iMod) == 0){
            std::cout << " -| " << std::setw(8) << std::fixed << (node+1) << "/" << num_nodes << std::endl; std::cout.flush();
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

                    selected  = true;
                }//if (ret == u_node.end()){//not found
            }while(!selected);
            //uint64_t iK = static_cast<uint64_t>(boost::out_degree(node, graph));
        }//for (uint64_t i = 0; i < m; i++){
        //--------------------------------------------------------------

    }//for (uint64_t node = m; node < num_nodes; node++){
}//void Scale_free_network(boost::adjacency_matrix<boost::directedS> &graph,


void Scale_free_network(boost::adjacency_matrix<boost::directedS> &graph,
                        uint64_t num_nodes, const double alpha_in, const double alpha_out){
//Algorithm based on paper named: Universal Behavior of Load Distribution in Scale-Free Networks
    std::default_random_engine gen;
    std::uniform_real_distribution<double> uniform(0,1);
    boost::container::vector <double> in_w_vet,
                                      out_w_vet,
                                      in_p_vet,
                                      out_p_vet;
    double sum_in = 0.0,
           sum_out = 0.0,
           pdf_in = 0.0,
           pdf_out = 0.0,
           add = 0.0;

    gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
    in_w_vet.resize(num_nodes);  out_w_vet.resize(num_nodes);
    in_p_vet.resize(num_nodes);  out_p_vet.resize(num_nodes);

    for (uint64_t i = 0; i < num_nodes; i++){
        double di = static_cast<double> (i+1);
        double pi = pow(di, -alpha_in);
        double qi = pow(di, -alpha_out);
        sum_in += pi;
        sum_out += qi;
        in_w_vet[i] = pi;
        out_w_vet[i] = qi;
    }//for (uint64_t i = 0; i < num_nodes; i++){

    for (uint64_t i = 0; i < num_nodes; i++){
        in_w_vet[i] /= sum_in;
        out_w_vet[i] /= sum_out;
    }//for (uint64_t i = 0; i < num_nodes; i++){

    pdf_in = in_w_vet[0]; pdf_out = out_w_vet[0];
    in_p_vet[0] = pdf_in;  out_p_vet[0] = pdf_out;
    for (uint64_t i = 1; i < num_nodes; i++){
        pdf_in += in_w_vet[i];
        pdf_out +=out_w_vet[i];
        in_p_vet[i] = pdf_in;
        out_p_vet[i] = pdf_out;
    }//for (uint64_t i = 0; i < num_nodes; i++){

    //for (uint64_t i = 0; i < num_nodes; i++) std::cout << "P_in(" << in_p_vet[i] << ") / P_out(" << out_p_vet[i] << ")" << std::endl;

    for (uint64_t node = 0; node < num_nodes; node++){
            bool selected = false;
            do{
                double p =  uniform(gen);
                add = in_p_vet[0];
                uint64_t j = 1;
                while (p > add)
                    add = in_p_vet[j++];
                j--;
                if (j != node){
                    add_edge(node, j, graph);
                    selected  = true;
                }
            }while(!selected);

            selected = false;
            do{
                double p =  uniform(gen);
                add = out_p_vet[0];
                uint64_t j = 1;
                while (p > add)
                    add = out_p_vet[j++];
                j--;
                if (j != node){
                    add_edge(j, node, graph);
                    selected  = true;
                }
            }while(!selected);

    }//for (uint64_t node = 0; node < num_nodes; node++){

}//void Scale_free_network(boost::adjacency_matrix<boost::directedS> &graph,

void Scale_free_network_v2_0(boost::adjacency_matrix<boost::directedS> &graph,
                        uint64_t num_nodes, const double alpha_in, const double alpha_out){
//Algorithm based on paper named: Universal Behavior of Load Distribution in Scale-Free Networks
    uint64_t max_nodes = num_nodes * 10;
    std::default_random_engine gen;
    std::uniform_real_distribution<double> uniform(0,1);
    boost::container::vector <double> in_w_vet,
                                      out_w_vet,
                                      in_p_vet,
                                      out_p_vet;

    boost::container::vector <uint64_t> unconnected;

    double sum_in = 0.0,
           sum_out = 0.0,
           pdf_in = 0.0,
           pdf_out = 0.0,
           add = 0.0;

    gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
    in_w_vet.resize(num_nodes);  out_w_vet.resize(num_nodes);
    in_p_vet.resize(num_nodes);  out_p_vet.resize(num_nodes);

    for (uint64_t i = 0; i < num_nodes; i++){
        double di = static_cast<double> (i+1);
        double pi = pow(di, -alpha_in);
        double qi = pow(di, -alpha_out);
        sum_in += pi;
        sum_out += qi;
        in_w_vet[i] = pi;
        out_w_vet[i] = qi;
    }//for (uint64_t i = 0; i < num_nodes; i++){

    for (uint64_t i = 0; i < num_nodes; i++){
        in_w_vet[i] /= sum_in;
        out_w_vet[i] /= sum_out;
    }//for (uint64_t i = 0; i < num_nodes; i++){

    pdf_in = in_w_vet[0]; pdf_out = out_w_vet[0];
    in_p_vet[0] = pdf_in;  out_p_vet[0] = pdf_out;
    for (uint64_t i = 1; i < num_nodes; i++){
        pdf_in += in_w_vet[i];
        pdf_out +=out_w_vet[i];
        in_p_vet[i] = pdf_in;
        out_p_vet[i] = pdf_out;
    }//for (uint64_t i = 0; i < num_nodes; i++){

    uint64_t iMod = max_nodes / 10;
    for (uint64_t n = 0; n < max_nodes; n++){

        if (((n+1) % iMod) == 0){
            std::cout << " -| " << std::setw(8) << std::fixed << (n+1) << "/" << max_nodes << std::endl;
            std::cout.flush();
        }

        bool equal = false;
        do{
            double pi = uniform(gen);
            double qi = uniform(gen);
            add = in_p_vet[0];
            uint64_t i = 1;
            while (pi > add)
                add = in_p_vet[i++];
            i--;

            add = out_p_vet[0];
            uint64_t j = 1;
            while (qi > add)
                add = out_p_vet[j++];
            j--;

            if (i != j){
                 //First, check if there is a previous edged connection between i -> j
                 auto edge_result = boost::edge(i, j, graph);
                 if (!edge_result.second) {
                    add_edge(i, j, graph);
                    equal = true;
                 }//if (edge_result.second) {
            }//if (i != j){
        }while(!equal);
    }//for (uint64_t k = 0; i < max_nodes; i++){

    unconnected.clear();
    for (uint64_t out_k = 0; out_k < num_nodes; out_k++){
        uint64_t index = static_cast<uint64_t>(boost::out_degree(out_k, graph));
        if (index == 0)
            unconnected.push_back(out_k);
    }//for (uint64_t k = 0; i < nodes; i++){
    //std::cout << unconnected.size() << std::endl;
    //for (uint64_t i = 0; i < num_nodes; i++) std::cout << "P_in(" << in_p_vet[i] << ") / P_out(" << out_p_vet[i] << ")" << std::endl;
    for (uint64_t node = 0; node < unconnected.size(); node++){
            bool selected = false;
            do{
                double p =  uniform(gen);
                add = in_p_vet[0];
                uint64_t j = 1;
                while (p > add)
                    add = in_p_vet[j++];
                j--;
                uint64_t i = unconnected[node];
                if (j != i){
                    auto edge_result = boost::edge(i, j, graph);
                    if (!edge_result.second) {
                        add_edge(i, j, graph);
                        selected  = true;
                    }//if (edge_result.second) {
                }//if (j != node){
            }while(!selected);

            selected = false;
            do{
                double p =  uniform(gen);
                add = out_p_vet[0];
                uint64_t j = 1;
                while (p > add)
                    add = out_p_vet[j++];
                j--;

                uint64_t i = unconnected[node];
                if (j != i){
                    auto edge_result = boost::edge(j, i, graph);
                    if (!edge_result.second) {
                        add_edge(j, i, graph);
                        selected  = true;
                    }//if (edge_result.second) {
                }
            }while(!selected);
    }//for (uint64_t node = 0; node < num_nodes; node++){
    unconnected.clear();
    for (uint64_t out_k = 0; out_k < num_nodes; out_k++){
        uint64_t index = static_cast<uint64_t>(boost::out_degree(out_k, graph));
        if (index == 0)
            unconnected.push_back(out_k);
    }//for (uint64_t k = 0; i < nodes; i++){
    std::cout << "Unconnected:" << unconnected.size() << std::endl;

}//void Scale_free_network(boost::adjacency_matrix<boost::directedS> &graph,