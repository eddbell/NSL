#include <iostream>
#include <fstream>
#include <string>
#include "../random/random.h"
#include "../block/block.h"
#include "main.h"
#include <armadillo>
#include <cmath>
using namespace std;
using namespace arma;

int main (int argc, char *argv[]){

  rnd.init_random(rnd);

  //###############ESERCIZIO 1.1.1########################################à

  for(int i = 0;i<M;i++){
    r(i) = rnd.Rannyu();
  }
  Block esercizio1(N,r);
  esercizio1.Average();
  mat_cum = esercizio1.Average_cumulative();

  mat_cum.save("../dati/dati1/dati1.1.1.dat", csv_ascii);
  //###############ESERCIZIO 1.1.2########################################à

  esercizio1.Varianza(1/2.);
  mat_cum = esercizio1.Average_cumulative();

  mat_cum.save("../dati/dati1/dati1.1.2.dat", csv_ascii);

  //###############ESERCIZIO 1.1.3########################################

  for(int j=0; j<N_times; j++){
   for(int i=0; i<N; i++){
     cou = 0;
     for(int k=0; k< n; k++){
       ran = rnd.Rannyu();
       if(ran>=i/double(N) && ran<(i+1)/double(N)){
         cou++;
       }
     }
     n_vec(i)=cou;
   }
   for(int i=0; i<N; i++){
     chi_cum[j] += ((n_vec[i]-n/double(N))*(n_vec[i]-n/double(N)))/(n/double(N));
   }
  }
  chi_cum.save("../dati/dati1/dati1.1.3.dat", csv_ascii);


   //###############ESERCIZIO 1.2########################################à

  for(int i =0; i<sizeN; i++ ){

    for(int j = 0; j<M; j++){
      t_n = 0;
      t_e = 0;
      t_l = 0;
      for(int k = 0; k<N_e(i); k++){
        t_n += rnd.Rannyu()/N_e(i);
        t_e += rnd.Exp(1)/N_e(i);
        t_l += rnd.Lorentz(1)/N_e(i);
      }
      nor(j,i) = t_n;
      esp(j,i) = t_e;
      lor(j,i) = t_l;
    }
  }
  nor.save("../dati/dati1/dati1.2.1.dat", csv_ascii);
  esp.save("../dati/dati1/dati1.2.2.dat", csv_ascii);
  lor.save("../dati/dati1/dati1.2.3.dat", csv_ascii);

  //###############ESERCIZIO 1.3########################################

  for(int i = 0; i<M;i++){
    cou=0;
    for(int j = 0; j<n_pi;j++){
      //simulo un bastone con rotazione random in un punto random
      x_1 = rnd.Rannyu(0,lspazio);
      y_1 = rnd.Rannyu(0,lspazio);
      x_2 = rnd.Rannyu(0,lspazio);
      y_2 = rnd.Rannyu(0,lspazio);
      if(x_1==x_2) y = y_1+l;
      else if(y_1==y_2) y = y_1;
      else{
        m = abs((y_2-y_1)/(x_2-x_1));
        y = y_1 + sqrt(m*m*l*l/(1+m*m));
      }
      //verifico se il bastone interseca le linee orizzontali distanti 1
      if(trunc(y) != trunc(y_1) ){
        cou++;
      }
    }
    pi_value[i]= 2.*l/(cou/n_pi);
  }


  //calcolo l'averages di ogni blocco
  Block pigreco(N,pi_value);
  pigreco.Average();
  mat_cum = pigreco.Average_cumulative();

  mat_cum.save("../dati/dati1/dati1.3.dat", csv_ascii);

  return 0;
}
