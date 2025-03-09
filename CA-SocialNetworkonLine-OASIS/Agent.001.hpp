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
        void update(void);
        void setState(unsigned int s)   { mState  = s; };
        void set_K_distance(float k)    { mKDist = k; }
        void setDelta(float d)          { mDelta = d; }
        void setAlpha(float a)          { mAlpha = a; }
        void setEpsilon(float e)        { mEpsilon = e; }
        void set_N_Neighbors(size_t s)  { mNNeightbors = s; };
        unsigned int getState(void)     { return mState; };
        size_t get_N_Neighbors(void)    { return mNNeightbors; }
        
        int setNeighborsAddr(Agent *);
        stList *getNeighborsList(void) { return mNeighbors; }
        void printNeighbors(void);
        void unaware2interested(void);
        //boost::container::vector<size_t> mNeighbors;
    private:
        void getInterestedAndAbandoned(size_t ,
        		float *,
				float *,
				boost::container::vector <unsigned int> *,
				stList *);


        stList *mNeighbors;
        //Agent **mNeighbors;
        size_t mNNeightbors;
        unsigned int mID;
        unsigned int mState;
        float mTime;
        float mKDist;
        float mDelta;
        float mAlpha;
        float mEpsilon;

};


#endif
