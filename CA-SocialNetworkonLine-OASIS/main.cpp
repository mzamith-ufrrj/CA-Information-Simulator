/*
 * File Main
 * Traffic Cellular Automata simulation  TCA-S
 */


#include <cstdlib>
#include <string>



#include <CellularAutomataI.hpp>
using namespace std;

int main(int ac, char **av){
   CellularAutomataI mCA;
   cout << "\t*** OASIS Model ***" << endl;
   mCA.loadConfig("config.json");
   mCA.print();
   mCA.exec();

   //mCA.initialCondition();
   //mCA.update();
   //mCA.statistic();

   //
   
   return EXIT_SUCCESS;
}

