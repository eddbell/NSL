#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include "../Libs/random/random.h"
#include "../Libs/block/block.h"
#include "main.h"

using namespace std;



int main (int argc, char *argv[]){

  auto S_t = [](double S0,double sigma,double ran_gauss,double r,double dt){
    return S0*exp((r-0.5*sigma*sigma)*dt+sigma*ran_gauss*sqrt(dt));
  };
  //###############ESERCIZIO 3.1########################################
  rnd.init_random();
  //***********ESERCIZIO 3.1.1*********************************************
  for( int i = 0;i<M;i++){
    S_final = S_t(S0,sigma,rnd.Gauss(ave,1),r,T);
    call_vec.push_back(exp(-r*T)*max(0.,S_final-K));
    put_vec.push_back(exp(-r*T)*max(0.,K-S_final));
  }
  Block cal_instant(N,call_vec);
  cal_instant.Average();
  cal_instant.Average_cumulative("./data/call1.dat");

  Block put_instant(N,put_vec);
  put_instant.Average();
  put_instant.Average_cumulative("./data/put1.dat");

  call_vec.clear();
  put_vec.clear();
  //***********ESERCIZIO 3.1.2*********************************************

  for( int i = 0;i<M;i++){
    S_final = S0;
    for(int j =0;j<1/dt;j++){
      S_final = S_t(S_final,sigma,rnd.Gauss(ave,1),r,dt);
    }
    call_vec.push_back(exp(-r*T)*max(0.,S_final-K));
    put_vec.push_back(exp(-r*T)*max(0.,K-S_final));
  }

  Block cal(N,call_vec);
  cal.Average();
  cal.Average_cumulative("./data/call2.dat");

  Block put(N,put_vec);
  put.Average();
  put.Average_cumulative("./data/put2.dat");

  return 0;
}
