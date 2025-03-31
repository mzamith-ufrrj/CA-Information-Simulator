#ifndef _CELLULAR_AUTOMATAI_HPP_
#define _CELLULAR_AUTOMATAI_HPP_
#include <iostream>
#include <Agent.hpp>
#include <iomanip>
#include <fstream>
#include <memory>
#include <random>
#include <string>
#include <boost/container/vector.hpp>
using namespace std;
class CellularAutomataI{
    public:
        enum STATES{
            ZEROSTATE,
            UNAWARE,
            CONCERNED,
            INTERESTED,
            ABANDONED
        };


        CellularAutomataI();
        ~CellularAutomataI();
        void loadConfig(const string &);
        void exec(void);
        void print(void);
        void debug(void);
        void debug1(void);
        static double getRandomNormalized(void);
        size_t getRandomInt(void);
        

    private:
		void initialCondition(void);
		void statistic(size_t = 0);
		void update(void);

        string mLogFile;
        size_t mNodeX;
        size_t mNodeY;
        size_t mNodes;
        size_t mKDist;
        size_t mMaxNeighbors;  /*!< It is a control variable, used to define the maximum number of neighbors for each Agent */
        size_t mTimeStep;

        double mP_Epsilon;
        double mN_Epsilon;
		double mAlpha;
        double mDelta;
        bool mVerbose;

        Agent **mAgents;
};
#endif
