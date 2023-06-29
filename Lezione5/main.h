#ifndef __main__
#define __main__
#include <armadillo>
#include "../random/random.h"

using namespace arma;
using namespace std;

//Random numbers
int seed[4];
Random rnd;

//Global variables

//constant
double a0 = 0.0529e-9;
int Ndim = 3;
// averages
double blk_av, blk_norm, accepted, attempted;
double glob_av, glob_av2;
double stima_radius, err_radius, r_ave;

//configuration
double delta;
vec x(Ndim);
// simulation
int iD,iGS, nstep, nblk, restart;
//*********Functions*************************************

void Input(void);
void Reset(int);
void Accumulate(void);
void Averages(int);
void Move(void);
void ConfFinal(void);

void Measure(void);
void Print(int);
double module(vec&);

double Error(double,double,int);
double GaussND(vec&);
double ESH1(vec&);
double ESH2(vec&);
/*
//calcola il modulo
double module(vec& v){
  double modux = 0;
  for(int i=0;i<int(v.n_elem);i++){
    modux += v[i]*v[i];
  }
  return sqrt(modux);
};

//funzioni algebriche
double eigenstateh2(vec& x1){
  return pow(a0,-3.)/M_PI*exp(-2*module(x1)/a0);
};

double n2eigenstateh2(vec& x1){
  return pow(a0,-3.)/M_PI*exp(-2*module(x1)/a0);
};
double gauss1D(double x1){
  return exp(-x1*x1);
};
double gaussND(vec& x1){
  return exp(-module(x1)*module(x1));
};

//metodi montecarlo
vec NDim_unif(vec& x0){
  vec x1(Ndim);
  double w;
  for(int i = 0;i<Ndim;i++){
    x1[i] = rnd.Rannyu(x0[i]-d,x0[i]+d);
    w = eigenstateh2(x1)/eigenstateh2(x0);
    cout<<w<<endl;
    if(w<1 && rnd.Rannyu()>w) x1 = x0;
  }
  return x1;
};

double A1D_unif(double x0){
  double x1 = rnd.Rannyu(x0-delta,x0+delta);
  //double x1 = rnd.Gauss(x0,d);
  double w = gauss1D(x1)/gauss1D(x0);
  if(w<1 && rnd.Rannyu()>w) x1 = x0;
  return x1;
};*/
#endif // __funzioni__
