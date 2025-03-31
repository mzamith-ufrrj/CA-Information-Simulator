#ifndef _AGENT_HPP_
#define _AGENT_HPP_
#include <cstdlib>
#include <cstdint>
//#include <boost/container/vector.hpp>

class Agent{
    public:
        Agent();
        ~Agent();
        unsigned int getID(void)        { return mID; };
        void setID(int64_t id)          { mID = id; }
        void setState(unsigned int s)   { mS1 = s;}
        unsigned int getState(void)     { return mS0; };
        void update(void);
        void setStates(unsigned int s = 0);
    private:
        int64_t  mID; 		    /*!< Agent ID > */
        unsigned int mS0;    /*!< State agent variable > */
        unsigned int mS1;
};


#endif
