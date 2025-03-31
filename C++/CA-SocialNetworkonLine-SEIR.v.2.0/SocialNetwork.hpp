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
        void createSpreader(uint64_t);
        void addSpreaders(uint64_t);
		void statistic(uint64_t = 0);
		void update(uint64_t = 0);
		double buildAlpha(uint64_t, uint64_t);
		double buildGamma(uint64_t,  uint64_t);
		void susceptible2NS(uint64_t, uint64_t);
		void exposed2NS(uint64_t , uint64_t);
		void infected2NS(uint64_t , uint64_t);

        std::string mLogFileStatistic; /*!< Statistic file name. CSV file */
        std::string mFileStateAgent;   /*!< Agent state file name. Showing Agents state. PPM file - Print as cellular automata */
        std::string mFileGraph;        /*!< File with graph built in another program. */

        uint64_t mN_Nodes;              /*!< Defined in mFileGraph file. Not JSON ones. */
        uint64_t mN_Spreaders;
        uint64_t mTimeStep;

        double mEpsilon;
        double mAveAlpha;
        double mN_Alpha;
        double mAveGamma;
        double mN_Gamma;
        double mK_Value;
        double *mVetProb;
        double mW_Alpha;
        double mW_Gamma;
        bool mVerbose;

        //boost::container::vector <uint64_t>  mSpreaders;
        boost::container::vector <Agent> mAgents;
        shared_ptr<boost::adjacency_matrix<boost::directedS>> mGraph;
        default_random_engine mGen;

        std::uniform_real_distribution<double> mUniform;
};
#endif
