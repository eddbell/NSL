/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include "random.h"
#include <armadillo>
#include <cmath>
using namespace std;
using namespace arma;



int main (int argc, char *argv[]){
  Random rnd;
  rnd.init_random(rnd);
  for(int i = 0; i<1000;i++){
    cout<<rnd.Rannyu()<<endl;
  }
  return 0;
}
