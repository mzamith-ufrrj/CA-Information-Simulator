/*
 * File Main
 * Traffic Cellular Automata simulation  TCA-S
 */


#include <cstdlib>
#include <string>



#include <SocialNetwork.hpp>
using namespace std;

int main(int ac, char **av){
   SocialNetwork mSN;
   cout << "\t*** SEIR Model ***" << endl;
   mSN.loadConfig(av[1]);

   mSN.initialCondition();
   mSN.print();
   //mSN.printNetworkStatistic();
   mSN.exec();

   //mCA.exec();
   //mCA.save2ppm();
   //mCA.initialCondition();
   //mCA.update();
   //mCA.statistic();

   //
   
   return EXIT_SUCCESS;
}

