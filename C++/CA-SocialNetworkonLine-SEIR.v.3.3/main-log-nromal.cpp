#include <iostream>
#include <random>

int main()
{
  const int nrolls = 10000; // number of experiments
  std::default_random_engine generator;
  std::poisson_distribution<int> distribution(4.1);



  for (int i=0; i<nrolls; ++i) {
    int number = distribution(generator);
    std::cout << i << " " << number << std::endl;
  }



  return 0;
}
