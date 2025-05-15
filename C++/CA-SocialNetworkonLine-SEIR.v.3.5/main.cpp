/*
 * File Main
 * Traffic Cellular Automata simulation  TCA-S
 */


#include <cstdlib>
#include <string>



#include <SocialNetwork.hpp>
using namespace std;

int main(int ac, char **av){
   SocialNetwork mSN = SocialNetwork();
   cout << "\t*** SEIR Model ***" << endl;
   mSN.initMem();
   mSN.loadConfig(av[1]);
   mSN.initialCondition();
   //mSN.printNetworkStatistic();
   mSN.exec();
   mSN.clearMem();
   //mCA.exec();
   //mCA.save2ppm();
   //mCA.initialCondition();
   //mCA.update();
   //mCA.statistic();

   //
   
   return EXIT_SUCCESS;
}

