#ifndef _AGENT_HPP_
#define _AGENT_HPP_
#include <cstdlib>
#include <boost/container/vector.hpp>
struct stList{
    void* agent;
    stList* next;
};
class Agent{
    public:
        Agent();
        ~Agent();
        unsigned int getID(void)        { return mID; };
        void setID(unsigned int id)     { mID = id; }
        void rule(void);
        void update(void);
		void setState(unsigned int s);
        void set_K_distance(double k)    { mKDist = k; }
        void setDelta(double d)          { mDelta = d; }
        void setAlpha(double a)          { mAlpha = a; }
        void setEpsilons(double pe, double ne)        { mP_Epsilon = pe; mN_Epsilon = ne; }
        void set_N_Neighbors(size_t s)  { mNNeightbors = s; };
        unsigned int getState(void)     { return mS0; };
        size_t get_N_Neighbors(void)    { return mNNeightbors; }
        double getElapsedTime(void)     { return mTime; }
        int setNeighborsAddr(Agent *);
        stList *getNeighborsList(void) { return mNeighbors; }
        void printNeighbors(void);


        //boost::container::vector<size_t> mNeighbors;
    private:
        void getInterestedAndAbandoned(size_t ,
        		double *,
				double *,
				boost::container::vector <unsigned int> *,
				stList *);

        void unaware2interested_concerned(void);
        void concerned2interested_abandoned(void);
        void interested2abandoned(void);

        stList *mNeighbors;
        //Agent **mNeighbors;
        size_t mNNeightbors;
        unsigned int mID; 		/*!< Agent ID > */
        unsigned int mS0;       /*!< State agent variable > */
        unsigned int mS1;
        double mTime;           /*!< Elapsed time of agent in INTERESTED state > */
        double mKDist;			/*!< K level used by model > */
        double mDelta;			/*!< It is threshold to change from INTERESTED to ABANDONED > */
        double mAlpha;			/*!< Infection rate > */
        double mP_Epsilon;      /*!< It is threshold to change from UNAWARE to CONCERNED(> mEpsilon) or  INTERESTED  (< -mEpsilon) */
        double mN_Epsilon;      /*!< It is threshold to change from UNAWARE to CONCERNED(> mEpsilon) or  INTERESTED  (< -mEpsilon) */
		double mMemory;         /*!< It represents the memory to change state based on mEpsilon. It can be positive or negative in according to the rule > */

};


#endif
