#ifndef _TOOLS_HPP_
#define _TOOLS_HPP_
#include <boost/container/vector.hpp>
#include <Agent.hpp>
enum STATES{ZEROSTATE, SUSCEPTIBLE, EXPOSED, INFECTED, RECOVERED};
//enum STATES{ZEROSATE, unaware, concerned, interested, abandoned};
void Scale_free_network(boost::container::vector <bool>*, uint64_t);
#endif
