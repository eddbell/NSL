#include <iostream>
#include <fstream>
#include <string>
#include <armadillo>
#include <iomanip>
#include <cmath>
#include "main.h"
using namespace std;
using namespace arma;

int main(int argc, char *argv[])
{
  //eseguo la simulazione
  Input();

  cout<<"-----------------------------------"<<endl;
  if (iD == 0) cout<<"UNIFORM STEP ----------------------"<<endl;
  else cout<<"GAUSSIAN STEP ---------------------"<<endl;
  if (iGS == 0) cout<<"--------------- ENERGY GROUND STATE"<<endl;
  else cout<<"---------------- ENERGY FIRST STATE"<<endl;
  cout<<"-----------------------------------"<<endl;

  ofstream Rad, Pos;
  Rad.open("output_rad.dat",ios::app);
  Pos.open("output_pos.dat",ios::app);
  double cum_av2 = 0;  double cum_ave = 0;  double cum_err = 0;

  for(int i = 0;i<N;i++){
    double sum_ave = 0;
    for(int j = 0;j< L;j++){
      sum_ave += module(x);
      x = NDmetropolis(x);
      for(int k = 0; k<Ndim;k++) Pos<<setw(15)<< x[k];
      if (iGS == 0) Pos<<setw(15)<<ESH1(x);
      else Pos<<setw(15)<<ESH2(x);
      Pos<<endl;
    }
    cum_ave = (cum_ave*i + sum_ave/L)/(i+1);
    cum_av2 = (cum_av2*i + sum_ave*sum_ave/L/L)/(i+1);
    cum_err = Error(cum_ave,cum_av2,i+1);

    cout<<"block "<<i+1<<"    "<<"acceptance rate: "<<(double)accepted/attempted<<endl;
    Rad<<i<<setw(15)<< cum_ave <<setw(15)<< cum_err <<endl;

  }

  Pos.close();
  Rad.close();
  return 0;
}

void Input(void)
{
  ifstream ReadInput, Primes, Seed;
  double x_in;
//Read seed for random numbers
  int p1, p2;
  Primes.open("Primes");
  Primes >> p1 >> p2 ;
  Primes.close();

//Read input informations
  ReadInput.open("input.in");

  ReadInput >> iD;  //Gauss o uniform?

  ReadInput >> iGS;  //Eigenstate

  ReadInput >> delta;//delta;
  ReadInput >> x_in;
  x.fill(x_in);

  ReadInput >> M;

  ReadInput >> N;
  L = M/N;
  ReadInput.close();

//Read seed input informations
  if(0) Seed.open("seed.out");
  else Seed.open("seed.in");
  Seed >> seed[0] >> seed[1] >> seed[2] >> seed[3];
  rnd.SetRandom(seed,p1,p2);
  Seed.close();
}

//algebric functions

double ESH1(vec& x1){
  return fabs(exp(-module(x1)));
};

double ESH2(vec& x1){
  return fabs(exp(-module(x1)/2)*pow(module(x1),2));
};
double GaussND(vec& x1){
  return exp(-(module(x1)-5)*(module(x1)-5));
};

//calcola il modulo
double module(vec& v){
  double mod = 0;
  for(int i=0;i<Ndim;i++){
    mod += v[i]*v[i];
  }
  return sqrt(mod);
};

void Print(int istep){
  ofstream Position;
  const int wd=30;
  Position.open("output_pos_iD_iGS.dat",ios::app);
  Position << istep;
  for(int i = 0; i< Ndim;i++) Position<<setw(wd)<<x[i];
  Position << endl;
  Position.close();
}

double Error(double sum, double sum2, int n)
{
    if(n==0) return 0.0;
    else return sqrt((sum2 - sum*sum)/n);
}

//metodi montecarlo
vec NDmetropolis(vec& x0){
  vec x1(Ndim);
  double w = 0;
  for(int i = 0;i<Ndim;i++){
    if (iD == 0)  x1[i] = x0[i]+rnd.Rannyu(-delta/2.,+delta/2.);
    else x1[i] = rnd.Gauss(x0[i],delta);
    if (iGS == 0)  w = ESH1(x1)/ESH1(x0);
    else  w = ESH2(x1)/ESH2(x0);
    w = min(1.,w*w);
    if (rnd.Rannyu() <= w)  accepted++;
    else x1 = x0;
    attempted++;
  }
  return x1;
};
