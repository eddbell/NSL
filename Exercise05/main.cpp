#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include "main.h"
#include "../Libs/random/random.h"
using namespace std;

int main(int argc, char *argv[])
{
  //eseguo la simulazione
  Input();

  cout<<"\n<INFO> Computing the comulative averages of the Radius: \n";
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
      if (iGS == 0) Pos<<setw(15)<<ESH1(module(x));
      else Pos<<setw(15)<<ESH2(x);
      Pos<<endl;
    }
    cum_ave = (cum_ave*i + sum_ave/L)/(i+1);
    cum_av2 = (cum_av2*i + sum_ave*sum_ave/L/L)/(i+1);
    cum_err = Error(cum_ave,cum_av2,i+1);

    if ( (i+1)% ((int)N/10)== 0) cout<<"Block "<<i+1<<"   Acceptance rate: "<<(double)accepted/attempted<<endl;
    Rad<<i<<setw(15)<< cum_ave <<setw(15)<< cum_err <<endl;

  }
  Pos.close();
  Rad.close();
  return 0;
}

void Input(void)
{
  ifstream ReadInput;
  double x_in;

  rnd.init_random();

//Read input informations
  ReadInput.open("input.in");

  ReadInput >> iD;  //Gauss o uniform?

  ReadInput >> iGS;  //Eigenstate

  ReadInput >> delta;//delta;

  ReadInput >> x_in;

  for(int i{};i<Ndim;i++) x[i] = x_in;

  ReadInput >> M; //steps

  ReadInput >> N; //blocks
  L = M/N;
  ReadInput.close();
}

//algebric functions

double ESH1(double r){
  return exp(-r)*exp(-r);
};

double ESH2(vector<double>& a_pos){
  double r=module(a_pos);
  double costheta=a_pos[2]/r;
  return fabs(exp(-r)*r*r*costheta*costheta);
};
double GaussND(double x1){
  return exp(-(x1-5)*(x1-5));
};

//calcola il modulo
double module(vector<double>& v){
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
    return (n==0 ? 0. : sqrt((sum2 - sum*sum)/n));
}

//metodi montecarlo
vector<double> NDmetropolis(vector<double>& x0){
  vector<double> x1(Ndim);
  double p_new = 0;
  double p_old = 0;

  if (iD == 0)  for(int i = 0;i<Ndim;i++) x1[i] = x0[i]+rnd.Rannyu(-delta/2.,+delta/2.);
  else for(int i = 0;i<Ndim;i++) x1[i] = rnd.Gauss(x0[i],delta);

  p_new = (iGS == 0 ? ESH1(module(x1)) : ESH2(x1));
  p_old = (iGS == 0 ? ESH1(module(x0)) : ESH2(x0));

  //w = p_new/p_old;
  attempted++;

  if (p_new > p_old){
    accepted++;
    return x1;
  }
  else{
    if (rnd.Rannyu()<p_new/p_old){
      accepted++;
      return x1;
    }
    return x0;
  }
};
