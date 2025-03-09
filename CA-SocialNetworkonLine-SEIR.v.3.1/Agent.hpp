#ifndef _AGENT_HPP_
#define _AGENT_HPP_
//#include <cstdlib>
#include <cstdint>
//#include <boost/container/vector.hpp>

class Agent{
    public:
        Agent();
        ~Agent();
        uint64_t getID(void)            { return mID; };
        void setID(int64_t id)          { mID = id; }
        void setState(uint64_t s)       { mS1 = s;}
        uint64_t getState(void)         { return mS0; };
        bool getSpreader(void)          { return mSpreader; };
        void update(void);
        void setStates(uint64_t s = 0);
        void setSpreader(void)            {mSpreader = true; };
    private:
        uint64_t  mID; 		    /*!< Agent ID > */
        uint64_t mS0;           /*!< State agent variable > */
        uint64_t mS1;
        bool mSpreader;
};


#endif
