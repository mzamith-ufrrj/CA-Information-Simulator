#ifndef _AGENT_HPP_
#define _AGENT_HPP_
//#include <cstdlib>
#include <cstdint>
//#include <boost/container/vector.hpp>
struct st_NeighborNode{
	uint64_t index;
	st_NeighborNode *next;
};

class Agent{
    public:
        Agent();
        ~Agent();
        void initMem(void);
        void clearMem(void);
        uint64_t getID(void)            { return mID; };
        void setID(int64_t id)          { mID = id; }
        void setState(uint64_t s)       { mS1 = s;}
        uint64_t getState(void)         { return mS0; };
        void setStates(uint64_t s = 0);
        void update(void);
        double getInDegree(void)        {return mInDegree;};
        double getOutDegree(void)       {return mOutDegree;};
        st_NeighborNode *getInList(void)  {return mInNode;}
        st_NeighborNode *getOutList(void)  {return mOutNode;}
        void addInNeighbor(uint64_t);
        void addOutNeighbor(uint64_t);
        void printNeighbors(void);
    private:
        void insert(st_NeighborNode **, st_NeighborNode*);
        uint64_t  mID; 		    /*!< Agent ID > */
        uint64_t mS0;           /*!< State agent variable > */
        uint64_t mS1;
        double mInDegree;
        double mOutDegree;
        st_NeighborNode *mInNode;
        st_NeighborNode *mOutNode;
};
void add_relation(uint64_t, uint64_t, Agent *, uint64_t);

#endif
