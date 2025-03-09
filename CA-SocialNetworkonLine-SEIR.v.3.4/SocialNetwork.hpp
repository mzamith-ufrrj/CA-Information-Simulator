#ifndef _SOCIAL_NETWORK_HPP_
#define _SOCIAL_NETWORK_HPP_
#include <Agent.hpp>

#include <cstdint>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>
#include <random>
#include <string>
#include <chrono>

using namespace std;
#define ERROR 1E-20
enum STATES {ZEROSTATE, SUSCEPTIBLE, EXPOSED, INFECTED, RECOVERED};
//enum INFECTED_NETWORK_ORDER {NO_ORDER, RANDOM_ORDER, ASCENDENT_ORDER, DESCENDENT_ORDER;}
class SocialNetwork{
    public:

        SocialNetwork();
        ~SocialNetwork();
        void initMem(void);
        void clearMem(void);
        void loadConfig(const std::string &);
        void initialCondition(void);
        void exec(void);
        void save2ppm(void);
        void printNetworkStatistic(void);

    private:
		void statistic(uint64_t = 0);
		void update(uint64_t = 0);
        double buildEpsilon(uint64_t, uint64_t);
		double buildAlpha(uint64_t, uint64_t);
		double buildGamma(uint64_t,  uint64_t);
		void susceptible2NS(uint64_t, uint64_t);
		void exposed2NS(uint64_t , uint64_t);
		void infected2NS(uint64_t , uint64_t);
		void print (void);

        std::string mLogFileStatistic; /*!< Statistic file name. CSV file */
        std::string mFileStateAgent;   /*!< Agent state file name. Showing Agents state. PPM file - Print as cellular automata */
        std::string mFileGraph;        /*!< File with graph built in another program. */
        std::string mInitNetworkOpt; /*!< Indicates how infected people are distributed. [rand, asc, desc] */

        uint64_t mN_Nodes;             /*!< Defined in mFileGraph file. Not JSON ones. */
        uint64_t mTimeStep;
        uint64_t mCellX;
        uint64_t mCellY;
        uint64_t mN_Infected;


	    double mP_Infected;
        double mAveAlpha;
        double mN_Alpha;
        double mAveGamma;
        double mN_Gamma;
        double mAveEpsilon;
        double mN_Epsilon;
        double *mVetProb;
        bool mVerbose;

        double mAlpha_Rumor_MI;
        double mAlpha_Rumor_SIG;
        double mGamma_Rumor_MI;
        double mGamma_Rumor_SIG;
        double mMaxOutDegree;
        double mMaxInDegree;
        double mAveOutDegree;
        double mAveInDegree;


        //boost::container::vector <uint64_t>  mSpreaders;
        Agent *mAgents;

        default_random_engine mGen;
        std::uniform_real_distribution<double> mUniform;
        //std::normal_distribution<double> mN_Alpha_Rumor, mN_Gamma_Rumor;
        std::lognormal_distribution<double> mN_Alpha_Rumor, mN_Gamma_Rumor;
};
#endif
