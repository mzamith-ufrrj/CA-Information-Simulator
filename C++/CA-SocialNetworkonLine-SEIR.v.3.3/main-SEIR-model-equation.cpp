//g++ main-SEIR-model-equation.cpp -o SEIR-Model
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cassert>
int main(int ac, char**av){
  double beta  = 0.3;
  double sigma = 0.1;
  double gamma =  0.05;

  double S1 = 0.0,
         S0 = 990.0,
         E1 = 0.0,
         E0 = 10.0,
         I1 = 0.0,
         I0 = 0.0,
         R1 = 0.0,
         R0 = 0.0,
         h  = 1E-2,
  	  	 N  = 1000.0;
  double  timestep = 100, t = 0.0;
  std::fstream output;
  std::cout << "SEIR - Equations" << std::endl;
  //std::cout << S1 << " " << I1 << " " << R1 << std::endl;
  output.open("SEIR-EQ-MODEL.txt",  std::fstream::trunc| std::fstream::out);
  assert(output.is_open());
  output << "0 " << S0 << " " << E0 << " " <<  I0 << " " << R0 << std::endl;
  t = 0.0;
  while (t <= timestep){
	  	double p1 = (beta * S0 * I0) / N;
	  	double p2 = sigma * E0;
	  	double p3 = gamma * I0;

	    S1 = S0 + h * -p1;

	    E1 = E0 + h * (p1 - p2);

	    I1 = I0 + h * (p2 - p3);
	    R1 = R0 + h * p3;

	    output << t << " " << S1 << " " << E1 << " " <<  I1 << " " << R1 << std::endl;

	    S0 = S1; I0 = I1; R0 = R1; E0 = E1;
	    t += h;
  }
  

  output.close();
  
  
  
  return EXIT_SUCCESS;
}
