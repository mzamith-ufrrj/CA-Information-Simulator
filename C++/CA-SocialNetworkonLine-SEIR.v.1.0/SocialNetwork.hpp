#ifndef _SOCIAL_NETWORK_HPP_
#define _SOCIAL_NETWORK_HPP_
#include <Agent.hpp>
#include <Tools.hpp>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>
#include <random>
#include <string>
#include <boost/container/vector.hpp>
#include <boost/range/algorithm/find.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/adjacency_matrix.hpp>
using namespace boost::container;
using namespace std;
class SocialNetwork{
    public:

        SocialNetwork();
        ~SocialNetwork();
        void loadConfig(const std::string &);
        void initialCondition(void);
        void exec(void);
        void save2ppm(void);
        void printNetworkStatistic(void);
        void print (void);
    private:

		void statistic(uint64_t = 0);
		void update(uint64_t = 0);
		double buildAlpha(uint64_t);
		double buldGamma(uint64_t,  uint64_t);
		void susceptible2NS(uint64_t);
		void exposed2NS(uint64_t , uint64_t);
		void infected2NS(uint64_t , uint64_t);




        std::string mLogFileStatistic; /*!< Statistic file name. CSV file */
        std::string mFileStateAgent;   /*!< Agent state file name. Showing Agents state. PPM file - Print as cellular automata */
        //std::string mStateFileGraph;   /*!< Graph file name. Binary, It is shown by python script. */
        std::string mFileGraph;        /*!< Graph histogram of in and out degree and its topology. */

        uint64_t mN_Nodes;
        uint64_t mX_Nodes;
        uint64_t mY_Nodes;

        uint64_t mN_Spreaders;
        uint64_t mTimeStep;



        double mAve_in_degree;
        double mAve_out_degree;
        double mMax_in_degree;
        double mMax_out_degree;
        double mEpsilon;
        double mAlpha;
        double mGamma;
        double mK_Value;

        bool mVerbose;
        boost::container::vector <uint64_t>  mSpreaders;
        boost::container::vector <Agent> mAgents;
        shared_ptr<boost::adjacency_matrix<boost::directedS>> mGraph;
        default_random_engine mGen;
        //std::normal_distribution<double> mNormal;
        std::uniform_real_distribution<double> mUniform;
};
#endif
