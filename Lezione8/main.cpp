#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <armadillo>

#include "main.h"
#include "../random/random.h"

using namespace std;
using namespace arma;


int main (int argc, char *argv[]){
  Input();
  //CAverage();
  //simulated annealing
  int nloop = 30; //cicli per beta value
  int ntot = 100; // numero di bet a value

  if(SA ==1){
    for (int i =0; i<ntot; i++){
      accepted = 0;
      attempted = 0;
      cout<<i<<" loop of "<<ntot<<endl;
      //while(attempted%nloop==0){
      for(int j = 0; j<nloop;j++){
        beta = 1+beta;
        mu += rnd.Rannyu(-1,1)/beta;
        sigma += rnd.Rannyu(-1,1)/beta;
        CAverage();
      }
    }
  }
  else CAverage();
}

double Error(double sum, double sum2, int n)
{
    if(n==0) return 0.0;
    else return sqrt((sum2 - sum*sum)/n);
}

void CAverage(){

  ofstream Ene, Pos;
  Ene.open("output_energy.dat",ios::app);
  Pos.open("output_position.dat",ios::app);
  double cum_av2 = 0;  double cum_ave = 0;  double cum_err = 0;

  for(int i = 0;i<N;i++){
    double sum_ave = 0;
    for(int j = 0;j< L;j++){
      sum_ave += Energy(x);
      x = Mstep(x);
    }
    cum_ave = (cum_ave*i + sum_ave/L)/(i+1);
    cum_av2 = (cum_av2*i + sum_ave*sum_ave/L/L)/(i+1);
    cum_err = Error(cum_ave,cum_av2,i+1);

    cout<<"block "<<i+1<<"    "<<"acceptance rate: "<<(double)accepted/attempted<<endl;
    //Ene<<i<<setw(15)<< cum_ave <<setw(15)<< cum_err <<endl;
  }

  Pos.close();
  Ene.close();
}

void Input(){
  ifstream Primes("Primes"), Seed, ReadInput;
  int p1, p2;
  Primes >> p1 >> p2 ;
  Primes.close();
  cout<<endl<<"---------------------------------------"<<endl;
  if(1) Seed.open("seed.in");
  else Seed.open("seed.out");
  Seed >> seed[0] >> seed[1] >> seed[2] >> seed[3];
  rnd.SetRandom(seed,p1,p2);
  Seed.close();
  cout<<"SETTING\n";
  ReadInput.open("input.in");
  ReadInput >> delta;
  cout<<"delta = "<<delta<<endl;
  ReadInput >> x;
  cout<<"x = "<<x<<endl;
  ReadInput >> sigma;
  ReadInput >> mu;
  cout<<"sigma = "<<sigma<<"     mu = "<<mu<<endl;
  ReadInput >> L;
  ReadInput >> N;
  cout<<"Lengh block = "<<L<<"    "<<"Number of blocks "<<N<<endl;
  ReadInput >> SA;
  ReadInput.close();
  cout<<endl<<"---------------------------------------"<<endl;
  accepted =0;
  attempted = 0;
}

double Mstep(double x0){
  double w;
  double x1 = rnd.Rannyu(x0-delta,x0+delta);
  if(SA == 1) w = exp(-beta*GS2(x1)/GS2(x0));
  else w = GS2(x1)/GS2(x0);
  if(w<1 && rnd.Rannyu()>w) x1 = x0;
  else accepted++;
  attempted++;
  return x1;
}

double Gauss1D(double x){
  return exp(-(x-mu)*(x-mu)/sigma/sigma/2);
}
double GS2(double x){
  return (Gauss1D(x)+ Gauss1D(-x))*(Gauss1D(+x)+ Gauss1D(-x));
}

double Energy(double x) {
  double a1 = Gauss1D(x)*((x-mu)*(x-mu)/pow(sigma,4)-1/pow(sigma,2));
  double a2 =  Gauss1D(-x)*((x+mu)*(x+mu)/pow(sigma,4)-1/pow(sigma,2));
  double b = Gauss1D(x)+Gauss1D(-x);
  double a = -(a1+a2)/2.;
  return (a+V(x))/b;
}

double V(double x){
  return x*x*x*x-5./2.*x*x;
}
/*
int main (int argc, char *argv[]){

  Input();

  CAverage();
}

double Error(double sum, double sum2, int n)
{
    if(n==0) return 0.0;
    else return sqrt((sum2 - sum*sum)/n);
}

void CAverage(){

  ofstream Ene, Pos;
  Ene.open("output_energy.dat",ios::app);
  Pos.open("output_position.dat",ios::app);
  double cum_av2 = 0;  double cum_ave = 0;  double cum_err = 0;

  for(int i = 0;i<N;i++){
    double sum_ave = 0;
    for(int j = 0;j< L;j++){
      sum_ave += Energy(x);
      x = Mstep(x);
    }
    cum_ave = (cum_ave + sum_ave/L)/(i+1);
    cum_av2 = (cum_av2 + sum_ave*sum_ave/L/L)/(i+1);
    cum_err = cum_err + Error(cum_ave,cum_av2,i+1);

    cout<<"block "<<i+1<<"    "<<"acceptance rate: "<<(double)accepted/attempted<<endl;
    Ene<<i<<setw(15)<< cum_ave <<setw(15)<<  cum_err<<endl;
  }

  Pos.close();
  Ene.close();
}*/
