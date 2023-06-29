#include <iostream>
#include <fstream>
#include <string>
#include <armadillo>
#include <cmath>
#include "../block/block.h"
#include "../random/random.h"
#include "main.h"

using namespace std;
using namespace arma;


int main (int argc, char *argv[]){

  //###############ESERCIZIO 2.1########################################
  rnd.init_random(rnd);
  //***********ESERCIZIO 2.1.1*********************************************
  for( int i = 0;i<M;i++){
    S_final = S_t(S0,sigma,rnd.Gauss(ave,1),r,T);
    call_vec[i] = exp(-r*T)*max(0.,S_final-K);
    put_vec[i] = exp(-r*T)*max(0.,K-S_final);
  }
  Block cal_instant(N,call_vec);
  cal_instant.Average();
  output_mat = cal_instant.Average_cumulative();
  output_mat.save("../dati/dati3/dati3.1.1.dat", csv_ascii);

  Block put_instant(N,put_vec);
  put_instant.Average();
  output_mat = put_instant.Average_cumulative();
  output_mat.save("../dati/dati3/dati3.1.2.dat", csv_ascii);


  //***********ESERCIZIO 2.1.2*********************************************

  for( int i = 0;i<M;i++){
    S_final = S0;
    for(int j =0;j<1/dt;j++){
      S_final = S_t(S_final,sigma,rnd.Gauss(ave,1),r,dt);
    }
    call_vec[i] = exp(-r*T)*max(0.,S_final-K);
    put_vec[i] = exp(-r*T)*max(0.,K-S_final);
  }

  Block cal(N,call_vec);
  cal.Average();
  output_mat = cal.Average_cumulative();
  output_mat.save("../dati/dati3/dati3.2.1.dat", csv_ascii);

  Block put(N,put_vec);
  put.Average();
  output_mat = put.Average_cumulative();
  output_mat.save("../dati/dati3/dati3.2.2.dat", csv_ascii);

  return 0;
}

double S_t(double S0,double sigma,double ran_gauss,double r,double dt){
  return S0*exp((r-0.5*sigma*sigma)*dt+sigma*ran_gauss*sqrt(dt));
}
