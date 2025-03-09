//g++ main-SIR-model-equation.cpp -o SIR-Model
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cassert>
int main(int ac, char**av){
  double beta = 1.1;
  double mi  =  0.1;
  double S1 = 0.0,
         S0 = 1.0,
         I1 = 0.0,
         I0 = 1E-2,
         R1 = 0.0,
         R0 = 0.0,
         N  = 0.0,
         h  = 0.5;
  int timestep = 100;
  std::fstream output;
  std::cout << "SIR - Equations" << std::endl;
  //std::cout << S1 << " " << I1 << " " << R1 << std::endl;
  output.open("SIR-EQ-MODEL.txt",  std::fstream::trunc| std::fstream::out);
  assert(output.is_open());
  for (int s = 0; s < timestep; s++){
    S1 = S0 + h * (-beta * S0 * I0);
    I1 = I0 + h * ((beta * S0 * I0) - (mi * I0));
    R1 = R0 + h * (mi * I0);
    
    output << s << " " << S1 << " " << I1 << " " << R1 << std::endl;
    
    S0 = S1; I0 = I1; R0 = R1;
  }
  
  output.close();
  
  
  
  return EXIT_SUCCESS;
}