#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "../Libs/random/random.h"
#include "../Libs/block/block.h"
#include "main.h"
#include "funzioni.h"

using namespace std;
int accept_reject(double x,double y){
  if(x>y) return(1);
  else  return(0);
};

int main (int argc, char *argv[]){

  //makeDir("./data");
  //###############ESERCIZIO 2.1########################################
  cout <<endl<< "ESERCIZIO 2.1 - Monte Carlo integration\n" << endl;
  rnd.init_random();

  //***********ESERCIZIO 2.1.1*********************************************
  cout <<endl<< "ESERCIZIO 2.1.1 - MC uniform\n" << endl;

  for(unsigned int i{}; i<M; i++){
    double x = rnd.Rannyu(0,1);
    double y = rnd.Rannyu(0,M_PI/2.);
    r.push_back((y<integranda(x) ? 1 : 0)*M_PI/2.);
  }

  MCb.set_vec(r);
  MCb.Average();
  MCb.Average_cumulative("./data/MC_uniform.dat");
  r.clear();
  cout << "___________________________________________________________________________________" << endl<< endl;
  //***********ESERCIZIO 2.1.2*****************************************
  cout <<endl<< "ESERCIZIO 2.1.2 - important sampling\n" << endl;
  //riempio il vettore r con distribuiti (Accept-Reject) secondo la PARABOLA per l'import sampling

  for (unsigned int i{};i<M;++i){
    double x = rnd.Rannyu(0,1);
    double y = rnd.Rannyu(0,3./2.);
    if(parabola(x)-y > 0) r.push_back(x);

  }
  for(unsigned int i=0;i<M;i++){
    r[i] = integranda(r[i])/parabola(r[i]);
  }

  MCb.set_vec(r);
  MCb.Average();
  MCb.Average_cumulative("./data/MC_IS_parabola.dat");
  r.clear();
  cout << "___________________________________________________________________________________" << endl<< endl;

  cout <<endl<< "BONUS - Linear\n" << endl;
  //uso una RETTA per import sampling
  for(unsigned int i=0;i<M;i++){
    r.push_back(retta_cum(rnd.Rannyu()));//inversa della cumulativa
    r[i] = integranda(r[i])/retta(r[i]);
  }

  MCb.set_vec(r);
  MCb.Average();
  MCb.Average_cumulative("./data/MC_IS_lin.dat");
  r.clear();
  cout << "___________________________________________________________________________________" << endl<< endl;

  //***********ESERCIZIO 2.2*****************************************
  cout <<endl<< "ESERCIZIO 2.2 - 3D Random walk \n" << endl;
  //scelgo il raggio t.c. lo jacobiano sin^2 sia integrato col metodo uniform MC
  vector<double> r2(M*N);
  /*
  for (unsigned int i{};i<M*N;i++){
    double x = rnd.Rannyu(0,M_PI);
    double y = rnd.Rannyu(0,1);
    if(sin(x)*sin(x)>y) r2[i]=x;
  }*/
  unsigned int co = 0;
  while(co!=M*N){
    double x = rnd.Rannyu(0,M_PI);
    double y = rnd.Rannyu(0,1);
    if(accept_reject(sin(x)*sin(x),y) == 1){
      r2[co] = x;
      co++;
    }
  }
  for(unsigned int j = 0; j<M;j++){
    for(unsigned int i = 0; i<N;i++){//step temporali
      ran1 = rnd.Rannyu();
      ran2 = rnd.Rannyu();
      asse = trunc(3*ran1); //stabilisce la direzione
      verso = -1+2*ran2; //stabilisce il verso

      r_vettore_d[asse] += (verso>0 ? +1 : -1);
      r_dis[j][i] = norma(r_vettore_d);

      polar(r2[i+j*N],2*M_PI*ran2,r_vettore_c);
      r_con[j][i] = norma(r_vettore_c);
    }
    for (unsigned int k{};k<3;k++){
      r_vettore_c[k] = 0;
      r_vettore_d[k] = 0;
    }
  }

  cout<<"<INFO> Computing the averages of the discrete Random Walk rays: ";
  string filename = "./data/RW_discrete.dat";
  ofstream outputFileDis(filename);    //print it in output file

  for(unsigned int i{}; i<N; i++){ //numero blocchi = numero step = 100 per semplicità
    //divido le simulazioni in 100 blocchi e costruisco la matrice #blocchi x #step
    for(unsigned int j{}; j<M;j++) r_vec_temp[j] = r_dis[j][i];

    Block MWb(N);
    MWb.set_vec(r_vec_temp);
    MWb.Average();
    pair<double, double> ray_val = MWb.RW_Ray_Average();

    outputFileDis << ray_val.first<< ","<< ray_val.second<< "\n";
  }
  cout<<" DONE!"<<endl;
  cout<<"<INFO> Saving output file in: "<<filename<<endl;

  cout<<"<INFO> Computing the averages of the continuum Random Walk rays: ";
  filename = "./data/RW_continuum.dat";
  ofstream outputFileCon(filename);    //print it in output file

  for(unsigned int i = 0; i<N; i++){
    for(unsigned int j{}; j<M;j++)  r_vec_temp[j] = r_con[j][i];

    Block MWa(N);
    MWa.set_vec(r_vec_temp);
    MWa.Average();
    pair<double, double> ray_val = MWa.RW_Ray_Average();

    outputFileCon << ray_val.first<< ","<< ray_val.second<< "\n";
    //cout<< ray_val.first<< ","<< ray_val.second<< "\n";
  }
  cout<<" DONE!"<<endl;
  cout<<"<INFO> Saving output file in: "<<filename<<endl;


  cout << "___________________________________________________________________________________" << endl<< endl;

  return 0;
}
