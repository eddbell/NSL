#include <iostream>
#include <fstream>
#include <string>
#include <armadillo>
#include <cmath>
#include "../random/random.h"
#include "../block/block.h"
#include "main.h"
#include "funzioni.h"

using namespace std;
using namespace arma;

int main (int argc, char *argv[]){

  //###############ESERCIZIO 2.1########################################
  rnd.init_random(rnd);
  //***********ESERCIZIO 2.1.1*********************************************

  for(int i=0; i<M; i++){
    r(i)=integranda(rnd.Rannyu());
  }

  MCb.set_vec(r);
  MCb.Average();
  MCb.Average_cumulative("../dati/dati2/dati2.1.1.dat");

  //***********ESERCIZIO 2.1.2*****************************************

  //riempio il vettore r con distribuiti (Accept-Reject) secondo la PARABOLA per l'import sampling
  int count = 0; double x,y;
  while(count!=M){
    x = rnd.Rannyu(0,1);
    y = rnd.Rannyu(0,3./2.);
    if(parabola(x)-y > 0){
      r(count) = x;
      count++;
    }
  }
  for(int i=0;i<M;i++){
    r(i) = integranda(r(i))/parabola(r(i));
  }

  MCb.set_vec(r);
  MCb.Average();
  MCb.Average_cumulative("../dati/dati2/dati2.1.2.dat");

  //uso una RETTA per import sampling
  for(int i=0;i<M;i++){
    r(i) = retta_cum(rnd.Rannyu());//inversa della cumulativa
    r(i) = integranda(r(i))/retta(r(i));
  }

  MCb.set_vec(r);
  MCb.Average();
  MCb.Average_cumulative("../dati/dati2/dati2.1.2.dat");

  //***********ESERCIZIO 2.2*****************************************

  while(co!=M*N){
    double x = rnd.Rannyu(0,M_PI);
    double y = rnd.Rannyu(0,1);
    if(accept_reject(sin(x)*sin(x),y) == 1){
      r2(co) = x;
      co++;
    }
  }

  for(int j = 0; j<M;j++){
    for(int i = 0; i<N;i++){
      ran1 = rnd.Rannyu();
      ran2 = rnd.Rannyu();
      asse = trunc(3*ran1); //stabilisce la direzione
      verso = -1+2*ran2; //stabilisce il verso

      if(verso>0) r_vettore_d(asse) = r_vettore_d(asse) +1;
      else r_vettore_d(asse) = r_vettore_d(asse) -1;

      r_vettore_c = polar(r2(i+j*N),2*M_PI*ran2,r_vettore_c);
      r_con(j,i) = norma(r_vettore_c);
      r_dis(j,i) = norma(r_vettore_d);
    }
    r_vettore_d.zeros();
    r_vettore_c.zeros();
  }

  for(int i = 0; i<N; i++){ //numero blocchi = numero step = 100 per semplicità
    //divido le simulazioni in 100 blocchi e costruisco la matrice #blocchi x #step
    r_vec_temp = r_dis.col(i);
    MWb.set_vec(r_vec_temp);
    MWb.Average();
    MWb.Average_blocks("../dati/dati2/dati2.2.1.dat");
  }

  for(int i = 0; i<N; i++){
    r_vec_temp = r_con.col(i);
    MWb.set_vec(r_vec_temp);
    MWb.Average();
    MWb.Average_blocks("../dati/dati2/dati2.2.2.dat");
  }
  return 0;
}
